#include "tablahash.h"
#include "asistente.h"

#include <iostream>

TablaHash::TablaHash(int tam) {
    capacidad = tam;
    elementos = 0;
    tabla = new Asistente[capacidad];
}

TablaHash::~TablaHash() {
    delete[] tabla;
}

int TablaHash::hash(int clave) {
    return abs(clave) % capacidad;
}

void TablaHash::insertar(Asistente a) {
    int indice = hash(a.dni);
    int i = 1;
    while (tabla[indice].dni != -1) {
        indice = (indice + i) % capacidad;
        i++;
    }
    tabla[indice] = a;
    elementos++;
}

bool TablaHash::buscar(int dni) {
    int indice = hash(dni);
    int i = 1;
    while (tabla[indice].dni != -1) {
        if (tabla[indice].dni == dni)
            return true;
        indice = (indice + i) % capacidad;
        i++;
    }
    return false;
}

Asistente TablaHash::obtenerAsistente(int dni) {
    int indice = hash(dni);
    int i = 1;
    while (tabla[indice].dni != -1) {
        if (tabla[indice].dni == dni)
            return tabla[indice];
        indice = (indice + i) % capacidad;
        i++;
    }
    return Asistente();
}

void TablaHash::mostrar() {
    std::cout << "\nContenido de la tabla hash:\n";
    for (int i = 0; i < capacidad; i++) {
        if (tabla[i].dni != -1) {
            std::cout << "Posicion " << i
                    << ": DNI=" << tabla[i].dni
                    << ", Nombre=" << tabla[i].nombre
                    << ", Prioridad=" << tabla[i].prioridad << std::endl;
        } else {
            std::cout << "Posicion " << i << ": Vacio" << std::endl;
        }
    }
}