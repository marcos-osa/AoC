#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

const int HASH_SIZE = 10007;  // Tamaño primo para mejor dispersión

// Nodo que almacena una coordenada X y su rango de Y
struct Node {
    long long x;          // Clave
    long long y_min;      // Y mínima para esta X
    long long y_max;      // Y máxima para esta X
};

// Tabla hash manual implementada con encadenamiento
struct ManualHashTable {
    std::vector<Node> buckets[HASH_SIZE];

    int hash_function(long long x) {
        return std::abs(x) % HASH_SIZE;
    }

    // Insertamos o actualizamos un punto
    void insert(long long x, long long y) {
        int index = hash_function(x);
        
        // Buscamos si ya existe X en este bucket
        for (auto &node : buckets[index]) {
            if (node.x == x) {
                // Actualizamos el rango de Y
                if (y < node.y_min) node.y_min = y;
                if (y > node.y_max) node.y_max = y;
                return;
            }
        }
        
        // Si no existe, lo agregamos
        buckets[index].push_back({x, y, y});
    }
};

int main() {
    ManualHashTable ht;
    std::ifstream archivo("input1.txt"); 

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir input.txt" << std::endl;
        return 1;
    }

    std::string linea;
    long long global_min_x = 2e18, global_max_x = -2e18;

    // Leemos cada punto del archivo
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        // Reemplazamos separadores por espacios para parsear fácilmente
        for (char &c : linea) {
            if (c == ',' || c == ';' || c == ':') c = ' ';
        }

        long long x, y;
        if (std::sscanf(linea.c_str(), "%lld %lld", &x, &y) == 2) {
            ht.insert(x, y);
            if (x < global_min_x) global_min_x = x;
            if (x > global_max_x) global_max_x = x;
        }
    }
    archivo.close();

    std::vector<Node*> all_nodes;
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (size_t j = 0; j < ht.buckets[i].size(); ++j) {
            all_nodes.push_back(&ht.buckets[i][j]);
        }
    }

    long long max_area = 0;
    // Comparamos cada par de columnas X diferentes
    for (size_t i = 0; i < all_nodes.size(); ++i) {
        for (size_t j = i + 1; j < all_nodes.size(); ++j) {
            // Ancho entre las dos columnas X
            long long width = std::abs(all_nodes[i]->x - all_nodes[j]->x);
            
            // Dos formas de calcular la altura
            long long h1 = std::abs(all_nodes[i]->y_max - all_nodes[j]->y_min);
            long long h2 = std::abs(all_nodes[i]->y_min - all_nodes[j]->y_max);
            
            // El área es ancho por la altura máxima
            long long area = width * std::max(h1, h2);
            if (area > max_area) max_area = area;
        }
    }

    std::cout << "Resultado final: " << max_area << std::endl;

    return 0;
}
