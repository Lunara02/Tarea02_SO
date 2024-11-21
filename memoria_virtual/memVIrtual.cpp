#include <iostream>
#include "memVirtual.h"
#include <map>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <queue>


//Tabla de paginas implementada como un std::map
std::map<int, int> tablaDePaginas;

//COLA PARA FIFO
std::queue<int> colaFIFO;

//////////////////////////////////////////////////////////FUNCIONES TABLA DE PAGINAS////////////////////////////////////////////////////////////////

//FUncion para determinar si se puede agregar pagina a tabla, si no ejecutar algoritmo de reemplazo
void manejar_nueva_pagina(std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::string& algoritmo, int& fallo_pagina, const std::vector<int> referencias, int indice_actual){
    if(buscarPagina(tabla, numeroPagina)){
        // -> si se está usando LRU y la página ya está en memoria, es necesario colocar la página al frente de lru_list
        std::cout << "Página " << numeroPagina << " ya está en memoria." << std::endl;
        return;
    }
    fallo_pagina++;

    // Si la tabla ya está llena
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


// Función para agregar una página
void agregarPagina(std::map<int, int>& tabla, int numeroPagina) {
    tabla[numeroPagina] = tabla.size(); // Agregar o actualizar la entrada
    std::cout << "Página " << numeroPagina << " asignada al marco " << tabla.size() << std::endl;
}

// Función para buscar una página
bool buscarPagina(const std::map<int, int>& tabla, int numeroPagina) {
    if (tabla.find(numeroPagina) != tabla.end()) { // Si la página está en el mapa
        std::cout << "Página " << numeroPagina << " encontrada en el marco " << tabla.at(numeroPagina) << std::endl;
        return true;
    } else {
        std::cout << "Página " << numeroPagina << " no encontrada en memoria." << std::endl;
        return false;
    }
}

// Función para eliminar una página
void eliminarPagina(std::map<int, int>& tabla, int numeroPagina) {
    tabla.erase(numeroPagina); // Eliminar la entrada
    std::cout << "Página " << numeroPagina << " eliminada de la tabla." << std::endl;
}

// Función para leer referencias desde un archivo y almacenarlas en un vector
std::vector<int> leerReferencias(const std::string& nombreArchivo) {
    std::vector<int> referencias;
    std::ifstream archivo(nombreArchivo); // Abrimos el archivo

    if (!archivo) { // Validar si el archivo pudo abrirse
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return referencias; // Retornamos un vector vacío si hay error
    }

    int referencia;
    while (archivo >> referencia) { // Leer cada número del archivo
        referencias.push_back(referencia); // Guardar en el vector
    }

    archivo.close(); // Cerrar el archivo
    return referencias;
}

// Función para imprimir la tabla de páginas
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

    // Leer el número de marcos desde los argumentos
    int maxMarcos = std::atoi(argv[1]);
    if (maxMarcos <= 0) {
        std::cerr << "Error: El número de marcos debe ser mayor a 0." << std::endl;
        return 1;
    }

    //Leer el algoritmo seleccionado
    std::string algoritmo = argv[2];
    if (algoritmo != "FIFO" && algoritmo != "Optimo" && algoritmo != "LRU" && algoritmo != "LRU_Reloj") {
        std::cerr << "Error: Algoritmo no válido. Use FIFO, Optimo, LRU o LRU_Reloj." << std::endl;
        return 1;
    }

    std::cout << "Número de marcos iniciales: " << maxMarcos << std::endl;
    std::cout << "Algoritmo seleccionado: " << algoritmo << std::endl;


    //Leer referencias
    std::string archivoReferencias = "referencias.txt";
    std::vector<int> referencias = leerReferencias(archivoReferencias);

    int fallo_pagina=0;

    //Manejar el ingreso de cada pagina de referencias
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