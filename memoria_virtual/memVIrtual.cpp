#include <iostream>
#include "memVirtual.h"
#include <map>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <queue>


std::map<int, int> tablaDePaginas;

//COLA PARA FIFO
std::queue<int> colaFIFO;

//////////////////////////////////////////////////////////FUNCIONES TABLA DE PAGINAS////////////////////////////////////////////////////////////////

void manejar_nueva_pagina(std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::string& algoritmo, int& fallo_pagina, const std::vector<int> referencias, int indice_actual){
    if(buscarPagina(tabla, numeroPagina)){
        std::cout << "Página " << numeroPagina << " ya está en memoria." << std::endl;
        return;
    }
    fallo_pagina++;

    if (tabla.size() >= maxMarcos) {
        if(!ejecutarAlgoritmoReemplazo(algoritmo, tabla, numeroPagina, maxMarcos, referencias, indice_actual)){
            std::cout << "Error, no se pudo reemplazar una pagina" << "\n";
        }
        return;
    }
    if(algoritmo == "FIFO"){
        colaFIFO.push(numeroPagina);
    }
    agregarPagina(tabla,numeroPagina);
    return;
}


void agregarPagina(std::map<int, int>& tabla, int numeroPagina) {
    tabla[numeroPagina] = tabla.size();
    std::cout << "Página " << numeroPagina << " asignada al marco " << tabla.size() << std::endl;
}

bool buscarPagina(const std::map<int, int>& tabla, int numeroPagina) {
    if (tabla.find(numeroPagina) != tabla.end()) {
        std::cout << "Página " << numeroPagina << " encontrada en el marco " << tabla.at(numeroPagina) << std::endl;
        return true;
    } else {
        std::cout << "Página " << numeroPagina << " no encontrada en memoria." << std::endl;
        return false;
    }
}

void eliminarPagina(std::map<int, int>& tabla, int numeroPagina) {
    tabla.erase(numeroPagina);
    std::cout << "Página " << numeroPagina << " eliminada de la tabla." << std::endl;
}


std::vector<int> leerReferencias(const std::string& nombreArchivo) {
    std::vector<int> referencias;
    std::ifstream archivo(nombreArchivo);

    if (!archivo) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return referencias;
    }

    int referencia;
    while (archivo >> referencia) {
        referencias.push_back(referencia);
    }

    archivo.close();
    return referencias;
}


void imprimirTabla(const std::map<int, int>& tabla) {
    if (tabla.empty()) {
        std::cout << "La tabla de páginas está vacía." << std::endl;
        return;
    }

    std::cout << "Contenido de la tabla de páginas:" << std::endl;
    std::cout << "Página Virtual -> Marco Físico" << std::endl;
    for (const auto& [paginaVirtual, marcoFisico] : tabla) {
        std::cout << paginaVirtual << " -> " << marcoFisico << std::endl;
    }
}


int main(int argc, char* argv[]) {
    if(argc<3){
        std::cerr << "Uso: " << argv[0] << " <numero_de_marcos> <algoritmo>" << std::endl;
        return 1;
    }

    int maxMarcos = std::atoi(argv[1]);
    if (maxMarcos <= 0) {
        std::cerr << "Error: El número de marcos debe ser mayor a 0." << std::endl;
        return 1;
    }

    std::string algoritmo = argv[2];
    if (algoritmo != "FIFO" && algoritmo != "Optimo" && algoritmo != "LRU" && algoritmo != "LRU_Reloj") {
        std::cerr << "Error: Algoritmo no válido. Use FIFO, Optimo, LRU o LRU_Reloj." << std::endl;
        return 1;
    }

    std::cout << "Número de marcos iniciales: " << maxMarcos << std::endl;
    std::cout << "Algoritmo seleccionado: " << algoritmo << std::endl;


    std::string archivoReferencias = "referencias.txt";
    std::vector<int> referencias = leerReferencias(archivoReferencias);

    int fallo_pagina=0;

    std::cout << "Referencias leídas:" << std::endl;

    int indice_actual=0;
    for (int numero_pagina_virtual : referencias) {
        manejar_nueva_pagina(tablaDePaginas,numero_pagina_virtual,maxMarcos,algoritmo,fallo_pagina,referencias,indice_actual);
        imprimirTabla(tablaDePaginas);
        indice_actual++;
    }
    std::cout << "Total fallo paginas: "<<fallo_pagina << std::endl;

    
    return 0;
}