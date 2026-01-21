#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Move {
    char dir;    // 'L' o 'R'
    int steps;
};

// Recursión que simula los movimientos del dial
int solveRec(const std::vector<Move>& moves, int index, int pos) {
    if (index >= moves.size()) return 0;
    
    int steps = moves[index].steps % 100;
    
    int newPos;
    if (moves[index].dir == 'L')
        newPos = (pos - steps + 100) % 100;
    else
        newPos = (pos + steps) % 100;
    
    int add = (newPos == 0) ? 1 : 0;
    return add + solveRec(moves, index + 1, newPos);
}

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir input.txt\n";
        return 1;
    }

    std::vector<Move> moves;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        moves.push_back({ line[0], std::stoi(line.substr(1)) });
    }
    
    int startPos = 50;
    int result = solveRec(moves, 0, startPos);
    
    std::cout << "Contraseña (recursiva): " << result << "\n";
    
    return 0;
}
