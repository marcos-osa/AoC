#include <iostream>
#include <fstream>
#include <string>
int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir input.txt\n";
        return 1;
    }
    int pos = 50; // pos inicial
    long long countZero = 0; // Contador de veces en 0, es decir, la respuesta real, ya que no es necesario saber en todas las posiciones en las que el dial se para.
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        char dir = line[0];
        int steps = std::stoi(line.substr(1));
        if (dir == 'L')
            pos = (pos - steps % 100 + 100) % 100;
        else
            pos = (pos + steps % 100) % 100;
        if (pos == 0)
            countZero++;}
    std::cout << "Contraseña: " << countZero << "\n";
    return 0;
}
