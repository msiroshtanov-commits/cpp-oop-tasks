#include <iostream>
#include <algorithm>

int main() {
    int x1 = 0;
    bool y = (x1 != 0);
    std::cout << "a) y = " << std::boolalpha << y << std::endl;

    int x2 = 5, y2 = 10;
    int z = std::max(x2, y2);
    std::cout << "б) z = " << z << std::endl;

    int x3 = 3, y3 = 8, z3 = 5;
    int w = std::max({x3, y3, z3});
    std::cout << "в) w = " << w << std::endl;

    int x4 = 0;
    std::cout << "г) x4 = " << x4 << std::endl;

    // д) Закоментовано, щоб уникнути Timed out в онлайн-компіляторі:
    // int x5 = 1;
    // while (++x5 > 0);
    std::cout << "д) Цикл очікує знакового переповнення int" << std::endl;

    return 0;
}