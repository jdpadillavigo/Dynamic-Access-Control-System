#ifndef HEAP_H
#define HEAP_H

#include "asistente.h"

int parent(int i);
int left(int i);
int right(int i);
void max_heapify(Asistente A[], int n, int i);
void build_max_heap(Asistente A[], int n);
void mostrarHeap(Asistente A[], int n);
Asistente extract_max(Asistente A[], int& n);
bool actualizar_prioridad(Asistente A[], int n, int dni, int nuevaPrioridad);

#endif