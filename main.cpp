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

			char opcion2;
			do {
				std::cout << "\n--- MENU ---\n";
				std::cout << "1. Registrar asistente\n";
				std::cout << "2. Mostrar tabla hash\n";
				std::cout << "3. Mostrar heap\n";
				std::cout << "4. Extraer siguiente acceso (maxima prioridad)\n";
				std::cout << "5. Actualizar prioridad de un asistente\n";
				std::cout << "6. Mostrar accesos por zona y hora (AVL)\n";
				std::cout << "7. Salir\n";
				std::cout << "\nSeleccione una opcion: ";
				std::cin >> opcion2;

				if (opcion2 == '1') {
					int dni, prioridad, horaHHMM = 0;
					std::string nombre, zona, horaStr;

					std::cout << "\nDNI: ";
					std::cin >> dni;
					std::cin.ignore();
					std::cout << "Nombre: ";
					getline(std::cin, nombre);
					std::cout << "Prioridad (5-VIP a 1-General): ";
					std::cin >> prioridad;
					std::cout << "Zona de ingreso: ";
					std::cin >> zona;

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

				} else if (opcion2 == '2') {
					th.mostrar();

				} else if (opcion2 == '3') {
					mostrarHeap(heap, nHeap);

				} else if (opcion2 == '4') {
					Asistente siguiente = extract_max(heap, nHeap);
					if (siguiente.dni != -1)
						std::cout << "\n>> Accede: " << siguiente.nombre << " (DNI: " << siguiente.dni << ", Prioridad: " << siguiente.prioridad << ")\n";
					else
						std::cout << "\nNo hay asistentes en espera.\n";

				} else if (opcion2 == '5') {
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
				}

				else if (opcion2 == '6') {
					arbolAccesos.mostrar();
				}
			} while (opcion2 != '7');

			if (heap != nullptr) {
				delete[] heap;
			}

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

			char opcion2;
			do {
				std::cout << "\n--- MENU ---\n";
				std::cout << "1. Mostrar tabla hash\n";
				std::cout << "2. Mostrar heap\n";
				std::cout << "3. Extraer siguiente acceso (maxima prioridad)\n";
				std::cout << "4. Actualizar prioridad de un asistente\n";
				std::cout << "5. Mostrar accesos por zona y hora (AVL)\n";
				std::cout << "6. Salir\n";
				std::cout << "\nSeleccione una opcion: ";
				std::cin >> opcion2;

				if (opcion2 == '1') {
					th.mostrar();

				} else if (opcion2 == '2') {
					mostrarHeap(heap, nHeap);

				} else if (opcion2 == '3') {
					Asistente siguiente = extract_max(heap, nHeap);
					if (siguiente.dni != -1)
						std::cout << "\n>> Accede: " << siguiente.nombre << " (DNI: " << siguiente.dni << ", Prioridad: " << siguiente.prioridad << ")\n";
					else
						std::cout << "\nNo hay asistentes en espera.\n";

				} else if (opcion2 == '4') {
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
				}

				else if (opcion2 == '5') {
					arbolAccesos.mostrar();
				}

			} while (opcion2 != '6');

			if (heap != nullptr) {
				delete[] heap;
			}

			exit(0);

		}
	} while (opcion != '3');

	return 0;
}