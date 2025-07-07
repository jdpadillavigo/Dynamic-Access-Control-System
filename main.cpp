#include "asistente.h"
#include "tablahash.h"
#include "heap.h"
#include "avl.h"

#include <cstdlib>
#include <cmath>
#include <fstream>

void generarCSV();
int contarAsistentesEnCSV(const std::string& nombreArchivo);
void cargarDesdeCSV(const std::string& nombreArchivo, TablaHash& th, Asistente heap[], int& nHeap, ArbolAVL& arbolAccesos);

void registrarAsistenteManual(TablaHash& th, Asistente* heap, int& nHeap, ArbolAVL& arbolAccesos) {
	int dni, prioridad = 0, horaHHMM = 0;
	std::string nombre, zona, horaStr;

	std::cout << "\nDNI: ";
	std::cin >> dni;
	std::cin.ignore();
	std::cout << "Nombre: ";
	getline(std::cin, nombre);
	std::cout << "\nPrioridades:\n\n5 -> VIP\n4 -> Personal medico\n3 -> Seguridad\n2 -> Discapacitados\n1 -> Publico general\n\n";
	do {
		std::cout << "Prioridad a asignar: ";
		std::cin >> prioridad;
		if (prioridad > 5 || prioridad < 1)
			std::cout << ">> Prioridad incorrecta. Ingresa un valor entre 1 y 5.\n";
	} while (prioridad > 5 || prioridad < 1);
	std::cout << "Zona de ingreso: ";
	std::cin.ignore();
	std::getline(std::cin, zona);

	bool horaValida = false;
	do {
		std::cout << "Hora de ingreso (formato HH:MM): ";
		std::cin >> horaStr;

		if (horaStr.length() == 5 && horaStr[2] == ':') {
			int horas = std::stoi(horaStr.substr(0, 2));
			int minutos = std::stoi(horaStr.substr(3, 2));

			if (horas >= 0 && horas <= 23 && minutos >= 0 && minutos <= 59) {
				horaHHMM = horas * 100 + minutos;
				horaValida = true;
			} else {
				std::cout << ">> Hora incorrecta. Ingrese valores entre 00:00 y 23:59.\n";
			}
		} else {
			std::cout << ">> Formato incorrecto. Use el formato HH:MM (ej: 14:30).\n";
		}
	} while (!horaValida);

	arbolAccesos.insertar(RegistroAcceso(zona, horaHHMM));

	if (!th.buscar(dni)) {
		Asistente nuevo(dni, nombre, prioridad);
		th.insertar(nuevo);
		heap[nHeap++] = nuevo;
		build_max_heap(heap, nHeap);
	} else {
		std::cout << "Este asistente ya fue registrado.\n";
	}
}

void menuOperativo(TablaHash& th, Asistente* heap, int& nHeap, ArbolAVL& arbolAccesos, bool permitirRegistroManual) {
	int opcion2;
	do {
		std::cout << "\n--- MENU ---\n";
		if (permitirRegistroManual)
			std::cout << "0. Registrar asistente\n";
		std::cout << "1. Mostrar Tabla Hash\n";
		std::cout << "2. Mostrar Heap\n";
		std::cout << "3. Mostrar AVL\n";
		std::cout << "4. Buscar asistente por DNI\n";
		std::cout << "5. Extraer siguiente acceso\n";
		std::cout << "6. Actualizar prioridad de un asistente\n";
		std::cout << "7. Mostrar los 5 siguientes asistentes por prioridad\n";
		std::cout << "8. Mostrar cantidad de asistentes en una hora establecida\n";
		std::cout << "9. Mostrar zona con mas asistentes\n";
		std::cout << "10. Salir\n";
		std::cout << "\nSeleccione una opcion: ";
		std::cin >> opcion2;

		if (permitirRegistroManual && opcion2 == 0) {
			registrarAsistenteManual(th, heap, nHeap, arbolAccesos);
		} else if (opcion2 == 1) {
			th.mostrar();
		} else if (opcion2 == 2) {
			if (nHeap == 0) {
		        std::cout << "\nNo hay asistentes registrados.\n";
		    } else {
		        std::cout << "\nHeap (por prioridad):\n";
		        mostrarHeap(heap, nHeap);
		    }
		} else if (opcion2 == 3) {
			arbolAccesos.mostrar();
		} else if (opcion2 == 4) {
			int dni;
			std::cout << "\nDNI a buscar: ";
			std::cin >> dni;
			if (!th.buscar(dni)) {
				std::cout << "\nEste asistente no fue registrado.\n";
			} else {
				std::cout << "\nSe ha encontrado al asistente:\n";
				Asistente buscado = th.obtenerAsistente(dni);
				std::cout << buscado.nombre << " (DNI: " << buscado.dni << ", Prioridad: " << buscado.prioridad << ")\n";
			}
		} else if (opcion2 == 5) {
			Asistente siguiente = extract_max(heap, nHeap);
			if (siguiente.dni != -1)
				std::cout << "\n>> Accede: " << siguiente.nombre << " (DNI: " << siguiente.dni << ", Prioridad: " << siguiente.prioridad << ")\n";
			else
				std::cout << "\nNo hay asistentes en espera.\n";
		} else if (opcion2 == 6) {
			int dni, nueva;
			std::cout << "\nDNI del asistente: ";
			std::cin >> dni;
			std::cout << "Nueva prioridad (5 a 1): ";
			std::cin >> nueva;

			if (actualizar_prioridad(heap, nHeap, dni, nueva)) {
				std::cout << "\nPrioridad actualizada satisfactoriamente.\n";
			} else {
				std::cout << "\nNo se encuentra el asistente en el heap.\n";
			}
		} else if (opcion2 == 7) {
		    int cantidadAMostrar = (nHeap < 5) ? nHeap : 5;
		    if (cantidadAMostrar == 0) {
		        std::cout << "\nNo hay asistentes para mostrar.\n";
		    } else {
		        std::cout << "\nLos " << cantidadAMostrar << " siguientes asistentes por prioridad:\n";
		        mostrarHeap(heap, cantidadAMostrar);
		    }
		} else if (opcion2 == 8) {
			std::string horaStr;
		    int horaHHMM;
		    std::cout << "\nIngrese la hora (formato HH:MM): ";
		    std::cin >> horaStr;
		
		    if (horaStr.length() == 5 && horaStr[2] == ':') {
		        int hh = std::stoi(horaStr.substr(0, 2));
		        int mm = std::stoi(horaStr.substr(3, 2));
		        if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59) {
		            horaHHMM = hh * 100 + mm;
		            std::cout << "\n";
		            arbolAccesos.mostrarAsistentesEnHora(horaHHMM);
		        } else {
		            std::cout << ">> Hora incorrecta. Debe estar entre 00:00 y 23:59.\n";
		        }
		    } else {
		        std::cout << ">> Formato incorrecto. Debe ser HH:MM.\n";
		    }
		} else if (opcion2 == 9) {
			std::string zonaMas = arbolAccesos.obtenerZonaConMasAsistentes();
    		std::cout << "\n>> Zona con mas asistentes: " << zonaMas << "\n";
		}
	} while (opcion2 != 10);
}

int main() {
	ArbolAVL arbolAccesos;

	char opcion;
	do {
		std::cout << "--- MENU DE INICIO ---\n";
		std::cout << "1. Asignar asistentes manualmente\n";
		std::cout << "2. Cargar asistentes desde archivo CSV\n";
		std::cout << "3. Salir\n";
		std::cout << "\nSeleccione una opcion: ";
		std::cin >> opcion;

		if (opcion == '1') {
			int cantAsistentes;
			std::cout << "\nCantidad de asistentes a asignar: ";
			std::cin >> cantAsistentes;

			TablaHash th(cantAsistentes * 2);
			Asistente* heap = new Asistente[cantAsistentes];
			int nHeap = 0;

			menuOperativo(th, heap, nHeap, arbolAccesos, true);

			delete[] heap;
			exit(0);

		} else if (opcion == '2') {
			generarCSV();

			std::string nombreArchivo;
			std::cout << "\nNombre del archivo CSV: ";
			std::cin >> nombreArchivo;

			std::ifstream archivo(nombreArchivo);
			if (!archivo.is_open()) {
				std::cerr << "\nNo se pudo abrir el archivo: " << nombreArchivo << "\n";
				return 0;
			}

			int cantAsistentes = contarAsistentesEnCSV(nombreArchivo);

			if (cantAsistentes == 0) {
				std::cerr << "No se encontraron asistentes en el archivo.\n";
				return 1;
			}

			TablaHash th(cantAsistentes * 2);
			Asistente* heap = new Asistente[cantAsistentes];
			int nHeap = 0;

			cargarDesdeCSV(nombreArchivo, th, heap, nHeap, arbolAccesos);

			menuOperativo(th, heap, nHeap, arbolAccesos, false);

			delete[] heap;
			exit(0);
		}
	} while (opcion != '3');

	return 0;
}
