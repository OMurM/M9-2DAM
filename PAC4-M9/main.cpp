#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <mutex>

using namespace std;

// Meta de la carrera
const int META = 50;

// Mutex para evitar conflictos al escribir en la consola
mutex mtx;

// Estructura para representar a cada animal
struct Animal {
    string nombre;
    int velocidad_base;
    int posicion;
};

// Función que ejecuta el hilo de cada animal
void correr(Animal &animal, bool &ganador) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, animal.velocidad_base);

    while (animal.posicion < META && !ganador) {
        this_thread::sleep_for(chrono::milliseconds(200));

        // Pausa o avance aleatorio
        int avance = dist(gen);
        
        // Simulación de una pausa inesperada en algunos momentos
        if (avance % 5 == 0) {
            avance = 0; // Pause
        }
        
        animal.posicion += avance;

        // Revisar si el animal ha ganado
        if (animal.posicion >= META) {
            mtx.lock();
            if (!ganador) {
                ganador = true;
                cout << "\n" << animal.nombre << " ha llegado a la meta y ha ganado la carrera!" << endl;
            }
            mtx.unlock();
        }
    }
}

// Función para mostrar el estado de la carrera
void mostrarCarrera(const vector<Animal> &animales) {
    mtx.lock();
    cout << "\033[2J\033[1;1H"; // Limpia la pantalla
    for (const auto &animal : animales) {
        cout << animal.nombre << ": ";
        int posicion_a_mostrar = min(animal.posicion, META);
        for (int i = 0; i < posicion_a_mostrar; i++) {
            cout << ".";
        }
        cout << " (" << animal.posicion << "/" << META << ")" << endl;
    }
    mtx.unlock();
}

int main() {
    // Animales
    Animal liebre{"Liebre", 5, 0};
    Animal tortuga{"Tortuga", 2, 0};
    Animal perro{"Perro", 3, 0};

    vector<Animal> animales = {liebre, tortuga, perro};

    vector<thread> hilos;

    bool ganador = false;

    // Crear y lanzar un hilo para cada animal
    for (auto &animal : animales) {
        hilos.push_back(thread(correr, ref(animal), ref(ganador)));
    }

    // Mostrar la carrera en progreso
    while (!ganador) {
        mostrarCarrera(animales);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    mostrarCarrera(animales);

    // Esperar a que terminen todos los hilos
    for (auto &hilo : hilos) {
        hilo.join();
    }

    return 0;
}
