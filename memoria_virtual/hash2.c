#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para los nodos de la lista enlazada
typedef struct Nodo {
    int pagina_virtual;
    int marco; // Marco físico donde está cargada la página
    struct Nodo* siguiente;
} Nodo;

// Estructura para la tabla hash
typedef struct {
    Nodo** buckets; // Array dinámico de punteros a listas enlazadas
    int tamano;     // Tamaño de la tabla hash (número de buckets)
} TablaHash;

// Función hash (módulo)
int funcion_hash(int pagina_virtual, int tamano) {
    return pagina_virtual % tamano;
}

// Inicializa la tabla hash
void inicializar_tabla(TablaHash* tabla, int tamano) {
    tabla->tamano = tamano;
    tabla->buckets = (Nodo*)malloc(tamano * sizeof(Nodo));
    for (int i = 0; i < tamano; i++) {
        tabla->buckets[i] = NULL;
    }
}

// Busca una página virtual en la tabla hash
int buscar_pagina(TablaHash* tabla, int pagina_virtual) {
    int indice = funcion_hash(pagina_virtual, tabla->tamano);
    Nodo* actual = tabla->buckets[indice];
    while (actual != NULL) {
        if (actual->pagina_virtual == pagina_virtual) {
            return 1; // Página encontrada
        }
        actual = actual->siguiente;
    }
    return 0; // Página no encontrada
}

// Inserta una página virtual en la tabla hash
int insertar_pagina(TablaHash* tabla, int pagina_virtual) {
    int indice = funcion_hash(pagina_virtual, tabla->tamano);
    if(tabla->buckets[indice]!=NULL){
        return 0;
    }

    // Crear un nuevo nodo para la página
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->pagina_virtual = pagina_virtual;
    nuevo->marco = indice; // El marco es el índice en la tabla
    nuevo->siguiente = tabla->buckets[indice];

    // Insertar el nodo al inicio de la lista enlazada del bucket
    tabla->buckets[indice] = nuevo;

    printf("Página %d asignada al marco %d (bucket %d).\n", pagina_virtual, indice, indice);
    return 1;
}

// Imprime la tabla hash (para depuración)
void imprimir_tabla(TablaHash* tabla) {
    for (int i = 0; i < tabla->tamano; i++) {
        printf("Bucket %d: ", i);
        Nodo* actual = tabla->buckets[i];
        while (actual != NULL) {
            printf("(%d -> %d) ", actual->pagina_virtual, actual->marco);
            actual = actual->siguiente;
        }
        printf("\n");
    }
}

// Libera la memoria de la tabla hash
void liberar_tabla(TablaHash* tabla) {
    for (int i = 0; i < tabla->tamano; i++) {
        Nodo* actual = tabla->buckets[i];
        while (actual != NULL) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(tabla->buckets);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <numero_marcos>\n", argv[0]);
        return 1;
    }

    int marcos = atoi(argv[1]); // Leer el número de marcos iniciales
    if (marcos <= 0) {
        printf("El número de marcos debe ser mayor a 0.\n");
        return 1;
    }

    TablaHash tabla;
    inicializar_tabla(&tabla, marcos);

    int referencias[] = {0, 1, 3, 4, 1, 2, 5, 1, 2, 3, 4}; // Referencias a páginas virtuales
    int num_referencias = sizeof(referencias) / sizeof(referencias[0]);

    for (int i = 0; i < num_referencias; i++) {
        int pagina = referencias[i];
        if (!buscar_pagina(&tabla, pagina)) {
            printf("FALLO DE PAGINA: Página %d\n", pagina);
            if(!insertar_pagina(&tabla, pagina)){
                printf("ALGORITMO DE REEMPLAZO para pagina: %d\n", pagina);
            }
        }
    }

    printf("\nEstado final de la tabla hash:\n");
    imprimir_tabla(&tabla);

    liberar_tabla(&tabla);
    return 0;
}