#include <iostream>
#include <string>

bool isPalindrome(const std::string& str) {
    int left = 0;
    int right = str.length() - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

int main() {
    std::string input;
    std::cout << "Введіть рядок: ";
    std::getline(std::cin, input);

    if (isPalindrome(input)) {
        std::cout << "Рядок є паліндромом." << std::endl;
    } else {
        std::cout << "Рядок НЕ є паліндромом." << std::endl;
    }

    return 0;
}