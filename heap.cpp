#include "heap.h"

#include <iostream>
#include <algorithm>

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void max_heapify(Asistente A[], int n, int i) {
    int largest = i;
    int l = left(i);
    int r = right(i);

    if (l < n && A[l].prioridad > A[largest].prioridad)
        largest = l;
    if (r < n && A[r].prioridad > A[largest].prioridad)
        largest = r;

    if (largest != i) {
        std::swap(A[i], A[largest]);
        max_heapify(A, n, largest);
    }
}

void build_max_heap(Asistente A[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        max_heapify(A, n, i);
}

void mostrarHeap(Asistente A[], int n) {
    Asistente* copia = new Asistente[n];
    std::copy(A, A + n, copia);
    int tam = n;

    while (tam > 0) {
        Asistente maximo = extract_max(copia, tam);
        std::cout << maximo.nombre << " (DNI: " << maximo.dni << ", Prioridad: " << maximo.prioridad << ")\n";
    }

    delete[] copia;
}

Asistente extract_max(Asistente A[], int& n) {
    if (n <= 0)
        return Asistente();
    Asistente maximo = A[0];
    A[0] = A[n - 1];
    n--;
    max_heapify(A, n, 0);
    return maximo;
}

bool actualizar_prioridad(Asistente A[], int n, int dni, int nuevaPrioridad) {
    for (int i = 0; i < n; i++) {
        if (A[i].dni == dni) {
            A[i].prioridad = nuevaPrioridad;
            build_max_heap(A, n);
            return true;
        }
    }
    return false;
}

void insertar_en_heap(Asistente A[], int& n, Asistente nuevo) {
    A[n] = nuevo;
    int i = n;
    n++;

    while (i > 0 && A[parent(i)].prioridad < A[i].prioridad) {
        std::swap(A[i], A[parent(i)]);
        i = parent(i);
    }
}