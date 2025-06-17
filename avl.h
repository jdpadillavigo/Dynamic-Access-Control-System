#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <string>
#include <sstream>

struct RegistroAcceso {
    std::string zona;
    int hora; // formato HHMM
    int cantidad;

    RegistroAcceso(std::string z = "", int h = 0, int c = 1)
        : zona(z), hora(h), cantidad(c) {}
};

struct NodoAVL {
    RegistroAcceso datos;
    int factor_balance;
    NodoAVL* izq;
    NodoAVL* der;
    NodoAVL* padre;

    NodoAVL(const RegistroAcceso& info);
};

class ArbolAVL {
	private:
	    NodoAVL* raiz;
	
	    NodoAVL* insertarRecursivo(NodoAVL* nodo, const RegistroAcceso& info);
	    int calcularAltura(NodoAVL* nodo);
	    void actualizarFactor(NodoAVL* nodo);
	    NodoAVL* rebalance(NodoAVL* nodo);
	    NodoAVL* rotarIzquierda(NodoAVL* nodo);
	    NodoAVL* rotarDerecha(NodoAVL* nodo);
	    void mostrarEchado(NodoAVL* nodo, int nivel);
	    int comparar(const RegistroAcceso& a, const RegistroAcceso& b);
	
	public:
	    ArbolAVL();
	    void insertar(const RegistroAcceso& info);
	    void mostrar();
};

std::string formatearHora(int horaHHMM);
void mostrarArbolConLineas(NodoAVL* nodo, const std::string& prefijo = "", bool esUltimo = true);

#endif