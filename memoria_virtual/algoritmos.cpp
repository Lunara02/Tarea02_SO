#include "memVirtual.h"
#include <iostream>
#include <map>
#include <vector>
#include <queue>

// Implementación del algoritmo Óptimo
bool reemplazoOptimo(std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::vector<int>& referencias,  int indice_actual) {
    std::cout << "[Optimo] Reemplazando página para la nueva página " << numeroPagina << std::endl;
    
    int paginaAReemplazar = -1;
    int maxDistancia = -1; //-1 significa que no se usara en el futuro
    int marco_Fisico=0; //Guardar marco fisico de la pagina que sera reemplazada

    for (const auto& [paginaVirtual, marcoFisico] : tabla) {
        int distancia = -1; // Si no se encuentra en las referencias futuras, permanece en -1
        marco_Fisico = marcoFisico;
        // Buscar cuándo se volvera a usar esta página
        for (int i = indice_actual + 1; i < referencias.size(); i++) {
            if (referencias[i] == paginaVirtual) {
                distancia = i - indice_actual;
                break; // Parar al encontrar la primera aparición
            }
        }
        // Si la pagina no se usara en el futuro, seleccionarla inmediatamente
        if (distancia == -1) {
            paginaAReemplazar = paginaVirtual;
            break;
        }

        // Actualizar la pagina que tiene la mayor distancia
        if (distancia > maxDistancia) {
            maxDistancia = distancia;
            paginaAReemplazar = paginaVirtual;
        }
    }
    
    // Reemplazar la pagina seleccionada
    if (paginaAReemplazar != -1) {
        tabla.erase(paginaAReemplazar); // Eliminar la pagina seleccionada
        tabla[numeroPagina] = marco_Fisico; // Agregar la nueva pagina
        std::cout << "Página " << paginaAReemplazar << " reemplazada por la página " << numeroPagina << std::endl;
        return true;
    }
    return false;
}

// Implementación del algoritmo FIFO
bool reemplazoFIFO(std::map<int, int>& tabla, int numeroPagina, int maxMarcos){
    std::cout << "[FIFO] Reemplazando página para la nueva página " << numeroPagina << std::endl;

    // Obtener la pagina que llego primero (al frente de la cola)
    int paginaAReemplazar = colaFIFO.front();
    colaFIFO.pop(); // Sacar la pagina de la cola

    // Obtener el marco fisico asociado
    int marcoFisico = tabla[paginaAReemplazar];

    // Reemplazar la página
    tabla.erase(paginaAReemplazar); // Eliminar la pagina más antigua
    tabla[numeroPagina] = marcoFisico; // Agregar la nueva pagina en el mismo marco
    colaFIFO.push(numeroPagina); // Agregar la nueva pagina al final de la cola

    std::cout << "Página " << paginaAReemplazar << " reemplazada por la página " << numeroPagina << std::endl;

    return true;
}

// Implementación del algoritmo LRU
bool reemplazoLRU(std::map<int, int>& tabla, int numeroPagina, int maxMarcos) {
    std::cout << "[LRU] Reemplazando página para la nueva página " << numeroPagina << std::endl;
    // Aquí debes implementar la lógica de LRU
    return true;
}

// Implementación del algoritmo LRU Reloj
bool reemplazoLRUReloj(std::map<int, int>& tabla, int numeroPagina, int maxMarcos) {
    std::cout << "[LRU_Reloj] Reemplazando página para la nueva página " << numeroPagina << std::endl;
    // Aquí debes implementar la lógica del algoritmo de reloj
    return true;
}

// Función para manejar el reemplazo basado en el algoritmo seleccionado
bool ejecutarAlgoritmoReemplazo(const std::string& algoritmo, std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::vector<int>&referencias,  int indice_actual) {
    if (algoritmo == "FIFO") {
        return reemplazoFIFO(tabla, numeroPagina, maxMarcos);
    } else if (algoritmo == "Optimo") {
        return reemplazoOptimo(tabla, numeroPagina, maxMarcos, referencias, indice_actual);
    } else if (algoritmo == "LRU") {
        return reemplazoLRU(tabla, numeroPagina, maxMarcos);
    } else if (algoritmo == "LRU_Reloj") {
        return reemplazoLRUReloj(tabla, numeroPagina, maxMarcos);
    } else {
        std::cerr << "Error: Algoritmo no implementado." << std::endl;
        return false; // Indica que el algoritmo no es válido
    }
}
