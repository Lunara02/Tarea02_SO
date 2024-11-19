#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Nodo para la lista enlazada en cada posición de la tabla hash
typedef struct Node {
    int num_pag_virtual;   // Número de página virtual
    int num_marco_pag;     // Número de marco asociado
    struct Node* next;     // Siguiente nodo en la lista enlazada
} Node;

// Estructura de la tabla hash
typedef struct HashTable {
    int size;         // Tamaño de la tabla hash
    Node** table;     // Arreglo dinámico de punteros a nodos
} HashTable;

// Función de hash
int hash_function(HashTable* ht, int key) {
    return key % ht->size;
}

// Crear un nodo
Node* create_node(int num_pag_virtual, int num_marco_pag) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->num_pag_virtual = num_pag_virtual;
    new_node->num_marco_pag = num_marco_pag;
    new_node->next = NULL;
    return new_node;
}

// Crear una tabla hash
HashTable* create_table(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = size;
    ht->table = (Node**)malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

// Insertar un par (num_pag_virtual, num_marco_pag) en la tabla hash
void insert(HashTable* ht, int num_pag_virtual, int num_marco_pag) {
    int index = hash_function(ht, num_pag_virtual);
    Node* new_node = create_node(num_pag_virtual, num_marco_pag);
    // Insertar al inicio de la lista enlazada
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
}

// Buscar un marco por el número de página virtual
int search(HashTable* ht, int num_pag_virtual) {
    int index = hash_function(ht, num_pag_virtual);
    Node* current = ht->table[index];
    while (current) {
        if (current->num_pag_virtual == num_pag_virtual) {
            return current->num_marco_pag; // Encontrado
        }
        current = current->next;
    }
    return -1; // No encontrado
}

// Imprimir la tabla hash
void print_table(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        printf("Index %d: ", i);
        Node* current = ht->table[i];
        while (current) {
            printf("(%d -> %d) ", current->num_pag_virtual, current->num_marco_pag);
            current = current->next;
        }
        printf("\n");
    }
}

// Liberar memoria de la tabla hash
void free_table(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

// Programa principal para probar la implementación
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <tamaño de la tabla>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    if (size <= 0) {
        printf("El tamaño de la tabla debe ser mayor que 0.\n");
        return 1;
    }

    HashTable* ht = create_table(size);

    // Insertar pares (num_pag_virtual, num_marco_pag)
    insert(ht, 5, 100);
    insert(ht, 15, 200);
    insert(ht, 25, 300);

    // Buscar en la tabla hash
    printf("Frame for virtual page 5: %d\n", search(ht, 5));
    printf("Frame for virtual page 15: %d\n", search(ht, 15));
    printf("Frame for virtual page 25: %d\n", search(ht, 25));
    printf("Frame for virtual page 10 (not in table): %d\n", search(ht, 10));

    // Imprimir la tabla hash
    print_table(ht);

    // Liberar memoria
    free_table(ht);

    return 0;
}
