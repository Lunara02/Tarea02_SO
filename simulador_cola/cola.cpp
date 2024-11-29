#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <time.h>

class ColaCircularDinamica {
private:
    std::vector<int> buffer;
    size_t front;
    size_t rear;
    size_t count;
    std::mutex mtx;
    std::condition_variable cv_prod;
    std::condition_variable cv_cons;
    std::ofstream logFile;

    void escribirLog(const std::string &mensaje) {
        logFile << mensaje << std::endl;
    }

    void duplicar() {
        std::string mensaje = "Duplicando tamaño del buffer... Tamaño actual: " + std::to_string(buffer.size());
        escribirLog(mensaje);
        size_t nuevoTam = buffer.size() * 2;
        std::vector<int> nuevoBuffer(nuevoTam);

        for (size_t i = 0; i < count; ++i) {
            nuevoBuffer[i] = buffer[(front + i) % buffer.size()];
        }

        buffer = std::move(nuevoBuffer);
        front = 0;
        rear = count;
        mensaje = "Nuevo tamaño del buffer: " + std::to_string(buffer.size());
        escribirLog(mensaje);
    }

    void reducir() {
        if (buffer.size() > 1) {
            std::string mensaje = "Reduciendo tamaño del buffer... Tamaño actual: " + std::to_string(buffer.size());
            escribirLog(mensaje);
            size_t nuevoTam = buffer.size() / 2;
            std::vector<int> nuevoBuffer(nuevoTam);

            for (size_t i = 0; i < count; ++i) {
                nuevoBuffer[i] = buffer[(front + i) % buffer.size()];
            }

            buffer = std::move(nuevoBuffer);
            front = 0;
            rear = count;
            mensaje = "Nuevo tamaño del buffer: " + std::to_string(buffer.size());
            escribirLog(mensaje);
        }
    }

public:
    ColaCircularDinamica(size_t tam_inicial) 
        : buffer(tam_inicial), front(0), rear(0), count(0) {
        logFile.open("log.txt", std::ios::out);
        if (!logFile) {
            std::cerr << "No se pudo abrir el archivo log.txt" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    ~ColaCircularDinamica() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void push(int dato) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_prod.wait(lock, [this] { return count < buffer.size(); });

        buffer[rear] = dato;
        rear = (rear + 1) % buffer.size();
        ++count;

        std::string mensaje = "Producido: " + std::to_string(dato) + " | Tamaño actual: " + std::to_string(count);
        escribirLog(mensaje);

        if (count == buffer.size()) {
            duplicar();
        }

        cv_cons.notify_one();
    }

    int pop(int tiempoEsperaMax) {
        std::unique_lock<std::mutex> lock(mtx);
        
        if (cv_cons.wait_for(lock, std::chrono::seconds(tiempoEsperaMax), [this] { return count > 0; })) {
            int dato = buffer[front];
            front = (front + 1) % buffer.size();
            --count;

            std::string mensaje = "Consumido: " + std::to_string(dato) + " | Tamaño actual: " + std::to_string(count);
            escribirLog(mensaje);

            if (buffer.size() > 1 && count <= buffer.size() / 4) {
                reducir();
            }

            cv_prod.notify_one();
            return dato;
        } else {
            std::string mensaje = "Consumidor alcanzó el tiempo de espera sin consumir nada.";
            escribirLog(mensaje);
            return -1; 
        }
    }

};

void productor(ColaCircularDinamica &cola, int id) {
    for (int i = 0; i < 5; ++i) {
        cola.push(i + id * 100);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50 + 1));
    }
}

void consumidor(ColaCircularDinamica &cola, int id, int tiempoEsperaMax) {
    while(1){
        int dato = cola.pop(tiempoEsperaMax);  
        if (dato == -1) {
            break;  
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50 + 1));
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int productores = 0, consumidores = 0, tam_inicial = 0, tiempo_espera = 0;
    int opt;

    while ((opt = getopt(argc, argv, "p:c:s:t:")) != -1) {
        switch (opt) {
            case 'p':
                productores = std::atoi(optarg);
                break;
            case 'c':
                consumidores = std::atoi(optarg);
                break;
            case 's':
                tam_inicial = std::atoi(optarg);
                break;
            case 't':
                tiempo_espera = std::atoi(optarg);
                break;
            default:
                std::cerr << "Uso: " << argv[0] << " -p productores -c consumidores -s tamaño_inicial -t tiempo_espera" << std::endl;
                return EXIT_FAILURE;
        }
    }
    printf("Productores = %d\nConsumidores = %d\nTamaño buffer = %d\nTiempo espera = %d\n", productores, consumidores, tam_inicial, tiempo_espera);

    if (productores <= 0 || consumidores <= 0 || tam_inicial <= 0 || tiempo_espera <= 0) {
        std::cerr << "Todos los parámetros deben ser positivos y mayores a cero." << std::endl;
        return EXIT_FAILURE;
    }

    ColaCircularDinamica cola(tam_inicial);

    std::vector<std::thread> hilos;
    for (int i = 0; i < productores; ++i) {
        hilos.emplace_back(productor, std::ref(cola), i + 1);
    }

    for (int i = 0; i < consumidores; ++i) {
        hilos.emplace_back(consumidor, std::ref(cola), i + 1, tiempo_espera);
    }

    for (auto &hilo : hilos) {
        hilo.join();
    }

    printf("Fin Simulacion\n");

    return 0;
}
