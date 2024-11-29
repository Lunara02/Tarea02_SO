#include "memVirtual.h"
#include <iostream>
#include <map>
#include <vector>
#include <queue>

bool reemplazoOptimo(std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::vector<int>& referencias,  int indice_actual) {
    std::cout << "[Optimo] Reemplazando página para la nueva página " << numeroPagina << std::endl;
    
    int paginaAReemplazar = -1;
    int maxDistancia = -1;
    int marco_Fisico=0;

    for (const auto& [paginaVirtual, marcoFisico] : tabla) {
        int distancia = -1;
        marco_Fisico = marcoFisico;
        for (int i = indice_actual + 1; i < referencias.size(); i++) {
            if (referencias[i] == paginaVirtual) {
                distancia = i - indice_actual;
                break;
            }
        }
        if (distancia == -1) {
            paginaAReemplazar = paginaVirtual;
            break;
        }

        if (distancia > maxDistancia) {
            maxDistancia = distancia;
            paginaAReemplazar = paginaVirtual;
        }
    }
    
    if (paginaAReemplazar != -1) {
        tabla.erase(paginaAReemplazar);
        tabla[numeroPagina] = marco_Fisico;
        std::cout << "Página " << paginaAReemplazar << " reemplazada por la página " << numeroPagina << std::endl;
        return true;
    }
    return false;
}

bool reemplazoFIFO(std::map<int, int>& tabla, int numeroPagina, int maxMarcos){
    std::cout << "[FIFO] Reemplazando página para la nueva página " << numeroPagina << std::endl;

    int paginaAReemplazar = colaFIFO.front();
    colaFIFO.pop();

    int marcoFisico = tabla[paginaAReemplazar];

    tabla.erase(paginaAReemplazar);
    tabla[numeroPagina] = marcoFisico;
    colaFIFO.push(numeroPagina);

    std::cout << "Página " << paginaAReemplazar << " reemplazada por la página " << numeroPagina << std::endl;

    return true;
}

bool reemplazoLRU(std::map<int, int>& tabla, int numeroPagina, int maxMarcos) {
    std::cout << "[LRU] Reemplazando página para la nueva página " << numeroPagina << std::endl;
    return true;
}

bool reemplazoLRUReloj(std::map<int, int>& tabla, int numeroPagina, int maxMarcos) {
    std::cout << "[LRU_Reloj] Reemplazando página para la nueva página " << numeroPagina << std::endl;
    return true;
}

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
        return false;
    }
}
