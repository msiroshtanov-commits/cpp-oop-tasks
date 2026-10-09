#include <iostream>

int main() {
    int a[] = {0, 5, 3, 8, 12, 7};
    int n = sizeof(a) / sizeof(a[0]);
    int x = 8;
    int i;

    for (i = 1; i < n && a[i] != x; ++i);

    if (i < n) {
        std::cout << "Елемент " << x << " знайдено за індексом: " << i << std::endl;
    } else {
        std::cout << "Елемент " << x << " не знайдено." << std::endl;
    }

    return 0;
}