#ifndef GENERAR_ASISTENTES_H
#define GENERAR_ASISTENTES_H

#include "asistente.h"
#include "tablahash.h"
#include "heap.h"
#include "avl.h"

#include <string>

std::string generarNombreCompleto();
std::string generarHora();
bool dniRepetido(int dni, int usados[], int usadosCount);
void generarCSV();
void cargarDesdeCSV(const std::string& nombreArchivo, TablaHash& th, Asistente heap[], int& nHeap, ArbolAVL& arbolAccesos);
int contarAsistentesEnCSV(const std::string& nombreArchivo);

#endif
