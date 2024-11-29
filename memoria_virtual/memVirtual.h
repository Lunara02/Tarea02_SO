#ifndef MEMVIRTUAL_H
#define MEMVIRTUAL_H

#include <map>
#include <vector>
#include <string>
#include <queue>

bool buscarPagina(const std::map<int, int>& tabla, int numeroPagina);
void manejar_nueva_pagina(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
void agregarPagina(std::map<int, int>& tabla, int numeroPagina);
std::vector<int> leerReferencias(const std::string& nombreArchivo);


bool reemplazoFIFO(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool  reemplazoOptimo(std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::vector<int>& referencias,  int indice_actual);
bool  reemplazoLRU(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool  reemplazoLRUReloj(std::map<int, int>& tabla, int numeroPagina, int maxMarcos);
bool ejecutarAlgoritmoReemplazo(const std::string& algoritmo, std::map<int, int>& tabla, int numeroPagina, int maxMarcos, const std::vector<int>&referencias,  int indice_actual);

extern std::queue<int> colaFIFO;

#endif