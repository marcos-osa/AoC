#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>

using namespace std;

// Estructura para representar un divisor en el árbol
struct NodoSplitter {
    int r, c;                    // Posición (fila, columna)
    NodoSplitter *izq, *der;    // Ramas izquierda y derecha

    NodoSplitter(int fila, int col) : r(fila), c(col), izq(nullptr), der(nullptr) {}
};

class LaboratorioTree {
public:
    vector<string> grid;
    set<pair<int, int>> visitados;  // Divisores ya visitados (para evitar ciclos)
    int filas, columnas;

    LaboratorioTree(vector<string> g) : grid(g) {
        filas = grid.size();
        columnas = (filas > 0) ? grid[0].size() : 0;
    }

    // Construye el árbol bajando verticalmente hasta encontrar divisores
    NodoSplitter* construirTrayectoria(int fila_inicio, int col) {
        if (col < 0 || col >= columnas || fila_inicio >= filas) return nullptr;

        // Bajamos en la columna buscando divisores
        for (int r = fila_inicio; r < filas; ++r) {
            if (grid[r][col] == '^') {
                // Si ya lo visitamos, no lo procesamos de nuevo
                if (visitados.count({r, col})) return nullptr;
                
                visitados.insert({r, col});
                NodoSplitter* nuevoNodo = new NodoSplitter(r, col);

                // Recursión: el rayo se divide en dos direcciones
                nuevoNodo->izq = construirTrayectoria(r + 1, col - 1);
                nuevoNodo->der = construirTrayectoria(r + 1, col + 1);

                return nuevoNodo; 
            }
        }
        return nullptr; 
    }
    
    // Simplemente contamos cuántos divisores visitamos
    int contarNodos() {
        return visitados.size();
    }
};

int main() {
    ifstream archivo("input.txt");
    vector<string> mapa;
    string linea;

    if (!archivo.is_open()) {
        cerr << "Error al abrir input.txt" << endl;
        return 1;
    }

    while (getline(archivo, linea)) {
        if (!linea.empty()) mapa.push_back(linea);
    }
    archivo.close();

    LaboratorioTree lab(mapa);
    
    int col_s = -1;
    for (int j = 0; j < lab.columnas; ++j) {
        if (mapa[0][j] == 'S') {
            col_s = j;
            break;
        }
    }

    if (col_s != -1) {
        lab.construirTrayectoria(1, col_s);
    }

    cout << "Total de splitters activados (nodos del árbol): " << lab.contarNodos() << endl;

    return 0;
}
