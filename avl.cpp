#include "avl.h"

NodoAVL::NodoAVL(const RegistroAcceso& info)
    : datos(info), factor_balance(0), izq(nullptr), der(nullptr), padre(nullptr) {}

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

void ArbolAVL::insertar(const RegistroAcceso& info) {
    raiz = insertarRecursivo(raiz, info);
}

int ArbolAVL::comparar(const RegistroAcceso& a, const RegistroAcceso& b) {
    if (a.hora != b.hora)
        return a.hora < b.hora ? -1 : 1;
    return 0;
}

NodoAVL* ArbolAVL::insertarRecursivo(NodoAVL* nodo, const RegistroAcceso& info) {
    if (!nodo)
        return new NodoAVL(info);

    int comp = comparar(info, nodo->datos);
    if (comp < 0) {
        nodo->izq = insertarRecursivo(nodo->izq, info);
        nodo->izq->padre = nodo;
    } else if (comp > 0) {
        nodo->der = insertarRecursivo(nodo->der, info);
        nodo->der->padre = nodo;
    } else {
        nodo->datos.cantidad++;
        return nodo;
    }

    actualizarFactor(nodo);
    return rebalance(nodo);
}

int ArbolAVL::calcularAltura(NodoAVL* nodo) {
    if (!nodo)
        return 0;
    int izq = calcularAltura(nodo->izq);
    int der = calcularAltura(nodo->der);
    return 1 + (izq > der ? izq : der);
}

void ArbolAVL::actualizarFactor(NodoAVL* nodo) {
    nodo->factor_balance = calcularAltura(nodo->izq) - calcularAltura(nodo->der);
}

NodoAVL* ArbolAVL::rebalance(NodoAVL* nodo) {
    actualizarFactor(nodo);

    if (nodo->factor_balance > 1) {
        if (nodo->izq && nodo->izq->factor_balance < 0)
            nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (nodo->factor_balance < -1) {
        if (nodo->der && nodo->der->factor_balance > 0)
            nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

NodoAVL* ArbolAVL::rotarIzquierda(NodoAVL* nodo) {
    NodoAVL* nuevaRaiz = nodo->der;
    nodo->der = nuevaRaiz->izq;
    if (nuevaRaiz->izq)
        nuevaRaiz->izq->padre = nodo;

    nuevaRaiz->izq = nodo;
    nuevaRaiz->padre = nodo->padre;
    nodo->padre = nuevaRaiz;

    actualizarFactor(nodo);
    actualizarFactor(nuevaRaiz);
    return nuevaRaiz;
}

NodoAVL* ArbolAVL::rotarDerecha(NodoAVL* nodo) {
    NodoAVL* nuevaRaiz = nodo->izq;
    nodo->izq = nuevaRaiz->der;
    if (nuevaRaiz->der)
        nuevaRaiz->der->padre = nodo;

    nuevaRaiz->der = nodo;
    nuevaRaiz->padre = nodo->padre;
    nodo->padre = nuevaRaiz;

    actualizarFactor(nodo);
    actualizarFactor(nuevaRaiz);
    return nuevaRaiz;
}

void ArbolAVL::mostrar() {
    if (!raiz) {
        std::cout << "\nEl arbol esta vacio.\n";
        return;
    }
    std::cout << "\n";
    mostrarArbolConLineas(raiz);
}

void mostrarArbolConLineas(NodoAVL* nodo, const std::string& prefijo, bool esUltimo) {
    if (nodo != nullptr) {
        std::cout << prefijo;

        if (esUltimo) {
            std::cout << "+-- ";
        } else {
            std::cout << "|-- ";
        }

        std::cout << nodo->datos.zona << " - " << formatearHora(nodo->datos.hora)
                  << " (Cantidad: " << nodo->datos.cantidad << ")\n";

        std::string nuevoPrefijo = prefijo + (esUltimo ? "    " : "|   ");

        if (nodo->izq || nodo->der) {
            mostrarArbolConLineas(nodo->izq, nuevoPrefijo, false);
            std::cout << "\n";
            mostrarArbolConLineas(nodo->der, nuevoPrefijo, true);
        }
    }
}

std::string formatearHora(int horaHHMM) {
    int hh = horaHHMM / 100;
    int mm = horaHHMM % 100;

    std::ostringstream horaFormateada;
    if (hh < 10)
        horaFormateada << "0";
    horaFormateada << hh << ":";

    if (mm < 10)
        horaFormateada << "0";
    horaFormateada << mm;

    return horaFormateada.str();
}

//void ArbolAVL::mostrarEchado(NodoAVL* nodo, int nivel) {
//    if (nodo) {
//        std::cout << "\n";
//        mostrarEchado(nodo->der, nivel + 1);
//        for (int i = 0; i < nivel; i++)
//            std::cout << "    ";
//        std::cout << nodo->datos.zona << " - " << formatearHora(nodo->datos.hora)
//                  << " (Cantidad: " << nodo->datos.cantidad << ")\n";
//        mostrarEchado(nodo->izq, nivel + 1);
//    }
//}

void ArbolAVL::mostrarAsistentesEnHora(int hora) {
    bool encontrado = false;
    recorrerPorHora(raiz, hora, encontrado);
    if (!encontrado)
        std::cout << "No se encontraron registros para la hora especificada.\n";
}

void ArbolAVL::recorrerPorHora(NodoAVL* nodo, int hora, bool& encontrado) {
    if (!nodo) return;
    recorrerPorHora(nodo->izq, hora, encontrado);
    if (nodo->datos.hora == hora) {
        std::cout << "Zona: " << nodo->datos.zona
                  << ", Cantidad: " << nodo->datos.cantidad
                  << ", Hora: " << formatearHora(nodo->datos.hora) << "\n";
        encontrado = true;
    }
    recorrerPorHora(nodo->der, hora, encontrado);
}

std::string ArbolAVL::obtenerZonaConMasAsistentes() {
    const int MAX_ZONAS = 100;
    std::string zonas[MAX_ZONAS];
    int cantidades[MAX_ZONAS] = {0};
    int totalZonas = 0;

    contarAsistentesPorZona(raiz, zonas, cantidades, totalZonas);

    if (totalZonas == 0) return "Sin datos";

    std::cout << "\nCantidad de asistentes por zona:\n";
    int maxIdx = 0;
    for (int i = 0; i < totalZonas; ++i) {
        std::cout << " - " << zonas[i] << ": " << cantidades[i] << " asistentes\n";
        if (cantidades[i] > cantidades[maxIdx])
            maxIdx = i;
    }

    return zonas[maxIdx] + " (" + std::to_string(cantidades[maxIdx]) + " asistentes)";
}

void ArbolAVL::contarAsistentesPorZona(NodoAVL* nodo, std::string zonas[], int cantidades[], int& totalZonas) {
    if (!nodo) return;

    contarAsistentesPorZona(nodo->izq, zonas, cantidades, totalZonas);

    int i;
    for (i = 0; i < totalZonas; ++i) {
        if (zonas[i] == nodo->datos.zona) {
            cantidades[i] += nodo->datos.cantidad;
            break;
        }
    }

    if (i == totalZonas && totalZonas < 100) {
        zonas[totalZonas] = nodo->datos.zona;
        cantidades[totalZonas] = nodo->datos.cantidad;
        totalZonas++;
    }

    contarAsistentesPorZona(nodo->der, zonas, cantidades, totalZonas);
}
