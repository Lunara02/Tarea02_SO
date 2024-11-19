#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Estructura para la cola circular dinámica
typedef struct {
    int *buffer;
    int capacity;
    int size;
    int front;
    int rear;
} CircularQueue;

// Estructura Monitor
typedef struct {
    CircularQueue queue;
    pthread_mutex_t mutex;
    sem_t items; // Para consumidores
    sem_t spaces; // Para productores
    int producers_active;
} Monitor;

// Funciones para la cola circular
void initQueue(CircularQueue *q, int initial_capacity) {
    q->buffer = malloc(initial_capacity * sizeof(int));
    q->capacity = initial_capacity;
    q->size = 0;
    q->front = 0;
    q->rear = -1;
}

void freeQueue(CircularQueue *q) {
    free(q->buffer);
}

bool isFull(CircularQueue *q) {
    return q->size == q->capacity;
}

bool isEmpty(CircularQueue *q) {
    return q->size == 0;
}

void enqueue(CircularQueue *q, int item) {
    q->rear = (q->rear + 1) % q->capacity;
    q->buffer[q->rear] = item;
    q->size++;
}

int dequeue(CircularQueue *q) {
    int item = q->buffer[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

void resizeQueue(CircularQueue *q, int new_capacity) {
    int *new_buffer = malloc(new_capacity * sizeof(int));
    for (int i = 0; i < q->size; i++) {
        new_buffer[i] = q->buffer[(q->front + i) % q->capacity];
    }
    free(q->buffer);
    q->buffer = new_buffer;
    q->capacity = new_capacity;
    q->front = 0;
    q->rear = q->size - 1;
}

// Funciones del Monitor
void monitorInit(Monitor *m, int initial_capacity, int producers) {
    initQueue(&m->queue, initial_capacity);
    pthread_mutex_init(&m->mutex, NULL);
    sem_init(&m->items, 0, 0);
    sem_init(&m->spaces, 0, initial_capacity);
    m->producers_active = producers;
}

void monitorDestroy(Monitor *m) {
    freeQueue(&m->queue);
    pthread_mutex_destroy(&m->mutex);
    sem_destroy(&m->items);
    sem_destroy(&m->spaces);
}

// Variables globales
Monitor monitor;
FILE *log_file;

// Función de productor
void *producer(void *arg) {
    int id = *(int *)arg;
    free(arg);
    for (int i = 0; i < 10; i++) { // Cada productor produce 10 elementos
        sem_wait(&monitor.spaces);
        pthread_mutex_lock(&monitor.mutex);

        // Agregar elemento a la cola
        int item = rand() % 100 + 1; 
        enqueue(&monitor.queue, item);
        fprintf(log_file, "Productor %d: produjo %d\n", id, item);

        // Verificar si la cola está llena para duplicar
        if (isFull(&monitor.queue)) {
            int new_capacity = monitor.queue.capacity * 2;
            resizeQueue(&monitor.queue, new_capacity);
            sem_init(&monitor.spaces, 0, new_capacity - monitor.queue.size);
            fprintf(log_file, "Cola duplicada a %d\n", new_capacity);
        }

        pthread_mutex_unlock(&monitor.mutex);
        sem_post(&monitor.items);
        usleep(rand() % 100000); // Simular tiempo de producción
    }

    pthread_mutex_lock(&monitor.mutex);
    monitor.producers_active--;
    pthread_mutex_unlock(&monitor.mutex);
    return NULL;
}

// Función de consumidor
void *consumer(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while (true) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1; // Tiempo máximo de espera

        if (sem_timedwait(&monitor.items, &ts) == -1) {
            pthread_mutex_lock(&monitor.mutex);
            if (monitor.producers_active == 0 && isEmpty(&monitor.queue)) {
                pthread_mutex_unlock(&monitor.mutex);
                break;
            }
            pthread_mutex_unlock(&monitor.mutex);
            continue;
        }

        pthread_mutex_lock(&monitor.mutex);

        // Extraer elemento de la cola
        int item = dequeue(&monitor.queue);
        fprintf(log_file, "Consumidor %d: consumió %d\n", id, item);

        // Verificar si la cola debe reducirse
        if (monitor.queue.size > 0 && monitor.queue.size <= monitor.queue.capacity / 4) {
            int new_capacity = monitor.queue.capacity / 2;
            resizeQueue(&monitor.queue, new_capacity);
            sem_init(&monitor.spaces, 0, new_capacity - monitor.queue.size);
            fprintf(log_file, "Cola reducida a %d\n", new_capacity);
        }

        pthread_mutex_unlock(&monitor.mutex);
        sem_post(&monitor.spaces);
        usleep(rand() % 150000); // Simular tiempo de consumo
    }
    return NULL;
}

// Función principal
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 9) {
        printf("Uso: %s -p <productores> -c <consumidores> -s <tam_inicial> -t <tiempo_espera>\n", argv[0]);
        return 1;
    }

    int producers = 0, consumers = 0, initial_capacity = 0, wait_time = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) producers = atoi(argv[++i]);
        else if (strcmp(argv[i], "-c") == 0) consumers = atoi(argv[++i]);
        else if (strcmp(argv[i], "-s") == 0) initial_capacity = atoi(argv[++i]);
        else if (strcmp(argv[i], "-t") == 0) wait_time = atoi(argv[++i]);
    }

    log_file = fopen("simulapc.log", "w");
    if (!log_file) {
        perror("Error abriendo el archivo de log");
        return 1;
    }

    monitorInit(&monitor, initial_capacity, producers);

    pthread_t producer_threads[producers];
    pthread_t consumer_threads[consumers];

    for (int i = 0; i < producers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producer_threads[i], NULL, producer, id);
    }

    for (int i = 0; i < consumers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, id);
    }

    for (int i = 0; i < producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    for (int i = 0; i < consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    monitorDestroy(&monitor);
    fclose(log_file);
    return 0;
}
