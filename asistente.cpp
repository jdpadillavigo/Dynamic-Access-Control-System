#include "asistente.h"

Asistente::Asistente() {
    dni = -1;
    nombre = "";
    prioridad = 0;
}

Asistente::Asistente(int d, std::string n, int p) {
    dni = d;
    nombre = n;
    prioridad = p;
}