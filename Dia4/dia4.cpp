#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Adyacente = cualquiera de las 8 celdas alrededor
const int DESP_FILA[8]    = {-1, -1, -1,  0, 0,  1, 1, 1};
const int DESP_COLUMNA[8] = {-1,  0,  1, -1, 1, -1, 0, 1};


// Posición en el mapa de un rollo 
struct PosicionEnMapa {
    int fila;
    int columna;
};

bool leerMapaDesdeArchivo(const string &nombreArchivo, vector<vector<char>> &mapa)
{
    ifstream fichero(nombreArchivo);
    if (!fichero.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << nombreArchivo << "'.\n";
        return false;
    }

    mapa.clear();

    string linea;
    while (getline(fichero, linea)) {
        if (linea.empty()) continue;
        // Convertimos la línea string en un vector<char> para representar una fila del mapa
        mapa.push_back(vector<char>(linea.begin(), linea.end()));
    }

    fichero.close();

    if (mapa.empty()) {
        cerr << "Error: el archivo está vacío o no tiene el formato esperado.\n";
        return false;
    }

    return true;
}

// Verifica que (fila, columna) esté dentro del mapa
bool estaDentroDelMapa(const vector<vector<char>> &mapa, int fila, int columna)
{
    if (fila < 0 || fila >= (int)mapa.size()) return false;
    if (columna < 0 || columna >= (int)mapa[fila].size()) return false;
    return true;
}

// Grafo modelado sobre la cuadrícula: vértices = '@', grado = nº de vecinos adyacentes
class GrafoDeRollosConGrados {
private:
    vector<PosicionEnMapa> verticesRollo;
    vector<int> gradoPorRollo;

public:
    GrafoDeRollosConGrados(const vector<vector<char>> &mapa)
    {
        // Recopilo todos los vértices (@)
        for (int f = 0; f < (int)mapa.size(); f++) {
            for (int c = 0; c < (int)mapa[f].size(); c++) {
                if (mapa[f][c] == '@') {
                    verticesRollo.push_back({f, c});
                }
            }
        }

        gradoPorRollo.assign(verticesRollo.size(), 0);

        // Calculo el grado de cada vértice
        for (int i = 0; i < (int)verticesRollo.size(); i++) {

            int filaActual = verticesRollo[i].fila;
            int colActual  = verticesRollo[i].columna;

            int contadorVecinos = 0;

            for (int k = 0; k < 8; k++) {
                int filaVecina = filaActual + DESP_FILA[k];
                int colVecina  = colActual  + DESP_COLUMNA[k];

                if (!estaDentroDelMapa(mapa, filaVecina, colVecina)) continue;

                if (mapa[filaVecina][colVecina] == '@') {
                    contadorVecinos++;
                }
            }


            gradoPorRollo[i] = contadorVecinos;
        }
    }

    int numeroDeVertices() const
    {
        return (int)verticesRollo.size();
    }


    int gradoDelVertice(int i) const
    {
        return gradoPorRollo[i];
    }
};

// Accesible si grado < 4
int contarRollosAccesibles(const vector<vector<char>> &mapa)
{
    GrafoDeRollosConGrados grafo(mapa);

    int contadorAccesibles = 0;
    for (int v = 0; v < grafo.numeroDeVertices(); v++) {
        if (grafo.gradoDelVertice(v) < 4) {
            contadorAccesibles++;
        }
    }
    return contadorAccesibles;
}

int main()
{
    vector<vector<char>> mapaDeRollos;

    if (!leerMapaDesdeArchivo("input.txt", mapaDeRollos)) {
        return 1;
    }

    cout << contarRollosAccesibles(mapaDeRollos) << "\n";
    return 0;
}
