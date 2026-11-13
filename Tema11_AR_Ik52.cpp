#include <iostream>
#include <string>

// Overloading by argument type.
void showModel(int vertices) {
    std::cout << "Vertices: " << vertices << '\n';
}
void showModel(double scale) {
    std::cout << "Scale: " << scale << '\n';
}
void showModel(const std::string& name) {
    std::cout << "Model: " << name << '\n';
}

struct Position { double x, y, z; };

// Overloading by argument count; all arguments have type double.
Position setPosition(double x) { return {x, 0.0, 0.0}; }
Position setPosition(double x, double y) { return {x, y, 0.0}; }
Position setPosition(double x, double y, double z) {
    return {x, y, z};
}

void printPosition(const Position& p) {
    std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")\n";
}

int main() {
    std::cout << "1. Overloading by argument type\n";
    showModel(1200);
    showModel(1.5);
    showModel(std::string("AR_Train"));

    std::cout << "\n2. Overloading by argument count\n";
    Position p1 = setPosition(2.0);
    Position p2 = setPosition(2.0, 3.0);
    Position p3 = setPosition(2.0, 3.0, 4.0);
    printPosition(p1);
    printPosition(p2);
    printPosition(p3);
    return 0;
}
