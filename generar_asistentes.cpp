#include "generar_asistentes.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>

std::string nombres[] = {
    "Juan", "Ana", "Luis", "Raquel", "Carlos", "Laura", "Pedro", "Nora",
    "Diego", "Clara", "Jorge", "Elena", "Miguel", "Paula", "Alberto", "Carmen",
    "David", "Marta", "Fernando", "Isabel"
};

std::string apellidos[] = {
    "Ramos", "Suarez", "Navarro", "Dominguez", "Iglesias", "Vargas", "Romero", "Torres",
    "Mendoza", "Vega", "Flores", "Castillo", "Ruiz", "Morales", "Ortiz", "Campos",
    "Silva", "Rivas", "Delgado", "Molina"
};

std::string zonas[] = {"ZonaA", "ZonaB", "ZonaC", "ZonaD"};

std::string generarNombreCompleto() {
    std::string nombre = nombres[rand() % (sizeof(nombres) / sizeof(std::string))];
    std::string apellido = apellidos[rand() % (sizeof(apellidos) / sizeof(std::string))];
    return nombre + " " + apellido;
}

std::string generarHora() {
    int h = rand() % 24;
    int m = rand() % 60;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", h, m);
    return std::string(buffer);
}

bool dniRepetido(int dni, int usados[], int usadosCount) {
    for (int i = 0; i < usadosCount; ++i) {
        if (usados[i] == dni)
            return true;
    }
    return false;
}

void generarCSV() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::string nombre_archivo = "asistentes.csv";
    std::ofstream archivo(nombre_archivo);
    
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escritura.\n";
        return;
    }

    archivo << "dni,nombre,prioridad,zona,hora\n";

    const int cantidad = 1000;
    
    int usados[cantidad];
    int usadosCount = 0;

    for (int i = 0; i < cantidad;) {
        int dni = 10000000 + rand() % 90000000;
        if (dniRepetido(dni, usados, usadosCount))
            continue;

        usados[usadosCount++] = dni;

        std::string nombre = generarNombreCompleto();
        int prioridad = 1 + rand() % 5;
        std::string zona = zonas[rand() % (sizeof(zonas) / sizeof(std::string))];
        std::string hora = generarHora();

        archivo << dni << "," << nombre << "," << prioridad << "," << zona << "," << hora << "\n";
        ++i;
    }

    archivo.close();
    std::cout << "\nArchivo " << nombre_archivo << " generado satisfactoriamente con " << cantidad << " asistentes.\n";
}

void cargarDesdeCSV(const std::string& nombreArchivo, TablaHash& th, Asistente heap[], int& nHeap, ArbolAVL& arbolAccesos) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << "\n";
        return;
    }

    std::string linea;
    std::getline(archivo, linea);
    std::cout << "\n";
    
    int ignoradosDNI = 0;
    int ignoradosHora = 0;

    while (std::getline(archivo, linea)) {
        std::cout << "Leyendo linea: " << linea << "\n";
        std::stringstream ss(linea);
        std::string campo;

        int dni, prioridad, horaHHMM;
        std::string nombre, zona, horaStr;

        std::getline(ss, campo, ',');
        dni = std::stoi(campo);

        std::getline(ss, nombre, ',');

        std::getline(ss, campo, ',');
        prioridad = std::stoi(campo);

        std::getline(ss, zona, ',');

        std::getline(ss, horaStr, ',');
        if (horaStr.length() == 5 && horaStr[2] == ':') {
            int horas = std::stoi(horaStr.substr(0, 2));
            int minutos = std::stoi(horaStr.substr(3, 2));
            if (horas >= 0 && horas <= 23 && minutos >= 0 && minutos <= 59) {
                horaHHMM = horas * 100 + minutos;
            } else {
                std::cerr << "Hora incorrecta en linea: " << linea << "\n";
                ignoradosHora++;
                continue;
            }
        } else {
            std::cerr << "Formato de hora incorrecto en linea: " << linea << "\n";
            ignoradosHora++;
            continue;
        }

        if (!th.buscar(dni)) {
            Asistente nuevo(dni, nombre, prioridad);
            th.insertar(nuevo);
            heap[nHeap++] = nuevo;
            build_max_heap(heap, nHeap);
            arbolAccesos.insertar(RegistroAcceso(zona, horaHHMM));
        } else {
            std::cerr << "DNI duplicado: " << dni << "\n";
            ignoradosDNI++;
        }
    }
    
    archivo.close();
    std::cout << "\nCarga masiva de asistentes completada.\n";
    std::cout << "\nAsistentes ignorados por DNI duplicado: " << ignoradosDNI << "\n";
    std::cout << "Asistentes ignorados por formato de hora incorrecto: " << ignoradosHora << "\n";
}

int contarAsistentesEnCSV(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << "\n";
        return 0;
    }

    std::string linea;
    std::getline(archivo, linea);

    int contador = 0;
    while (std::getline(archivo, linea)) {
        if (!linea.empty()) {
            contador++;
        }
    }

    archivo.close();
    return contador;
}
