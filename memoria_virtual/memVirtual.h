#ifndef MEMVIRTUAL_H
#define MEMVIRTUAL_H

#include <map>
#include <vector>
#include <string>

// Declaraciones de funciones
bool buscarPagina(const std::map<int, int>& tabla, int numeroPagina);
void manejar_nueva_pagina(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
void agregarPagina(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
std::vector<int> leerReferencias(const std::string& nombreArchivo);

// Declaraciones de los algoritmos
bool reemplazoFIFO(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool  reemplazoOptimo(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool  reemplazoLRU(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool  reemplazoLRUReloj(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool  ejecutarAlgoritmoReemplazo(const std::string& algoritmo, std::map<int, int>& tabla, int numeroPagina, int maxMarcos);

#endif // MEMVIRTUAL_H