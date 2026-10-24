#include <iostream>
#include <string>

template <typename T, typename U>
void printValues(const T& first, const U& second)
{
    std::cout << "Argument 1: " << first << '\n';
    std::cout << "Argument 2: " << second << "\n\n";
}

int main()
{
    printValues(15, 3.5);
    printValues(std::string("Hello"), 2026);
    printValues('A', std::string("OOP"));

    return 0;
}