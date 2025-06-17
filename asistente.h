#ifndef ASISTENTE_H
#define ASISTENTE_H

#include <string>

struct Asistente {
    int dni;
    std::string nombre;
    int prioridad;

    Asistente();
    Asistente(int d, std::string n, int p);
};

#endif