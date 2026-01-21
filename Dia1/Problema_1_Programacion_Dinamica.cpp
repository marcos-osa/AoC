#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Movimiento: dirección (L/R) y número de pasos
struct Movimiento {
    char direccion;
    int pasos;
};

// Tabla para guardar resultados ya calculados
std::vector<std::vector<int>> memo;

// Recursión con memoización: evita recalcular los mismos estados
int resolverDP(const std::vector<Movimiento>& movimientos,
               int indice,
               int posicion) {

    // Si no hay más movimientos, terminamos
    if (indice >= movimientos.size()) return 0;

    // Si ya calculamos este estado, lo devolvemos directamente
    if (memo[indice][posicion] != -1) 
        return memo[indice][posicion];

    // Calculamos los pasos módulo 100
    int pasos = movimientos[indice].pasos % 100;

    // Nueva posición según la dirección
    int nuevaPosicion;
    if (movimientos[indice].direccion == 'L')
        nuevaPosicion = (posicion - pasos + 100) % 100;  // Izquierda (resta)
    else
        nuevaPosicion = (posicion + pasos) % 100;        // Derecha (suma)

    // Contamos si llegamos a 0
    int suma = (nuevaPosicion == 0) ? 1 : 0;

    // Guardamos en memo y devolvemos
    return memo[indice][posicion] = suma + resolverDP(movimientos, indice + 1, nuevaPosicion);
}

int main() {
    std::ifstream archivo("input.txt");
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir input.txt\n";
        return 1;
    }

    std::vector<Movimiento> movimientos;
    std::string linea;

    // Leemos todos los movimientos
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        movimientos.push_back({ linea[0], std::stoi(linea.substr(1)) });
    }

    // Inicializamos la tabla de memoización
    memo.assign(movimientos.size(), std::vector<int>(100, -1));

    int posicionInicial = 50;
    int resultado = resolverDP(movimientos, 0, posicionInicial);

    std::cout << "Contraseña (DP con Matriz): " << resultado << "\n";
    return 0;
}
