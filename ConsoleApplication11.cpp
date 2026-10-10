#include <iostream>
#include <string>
using namespace std;

int main() {
    string students[10] = {
        "Maksym", "Andrii", "Dmytro",
        "Ivan", "Oleh", "Bohdan",
        "Anna", "Olena", "Iryna", "Sofiia"
    };

    for (int i = 0; i < 10; ++i)
        cout << students[i] << endl;

    return 0;
}