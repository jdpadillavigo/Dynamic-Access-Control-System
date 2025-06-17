#ifndef TABLAHASH_H
#define TABLAHASH_H

#include "asistente.h"

#include <iostream>

class TablaHash {
    private:
        int capacidad;
        int elementos;
        Asistente* tabla;

    public:
        TablaHash(int tam);
        ~TablaHash();

        int hash(int clave);
        void insertar(Asistente a);
        bool buscar(int dni);
        Asistente obtenerAsistente(int dni);
        void mostrar();
};

#endif