#include <iostream>
using namespace std;
class TwoNumbers {
    double first, second;
public:
    TwoNumbers(double a, double b) : first(a), second(b) {}
    void setValues(double a, double b) {
        first = a;
        second = b;
    }
    void print() const {
        cout << "Values: " << first << " " << second << '\n';
    }
    double sum() const { return first + second; }
    double maximum() const {
        return first >= second ? first : second;
    }
};
int main() {
    TwoNumbers numbers(7.5, -2);
    numbers.print();
    cout << "Sum: " << numbers.sum() << '\n';
    cout << "Maximum: " << numbers.maximum() << '\n';
    numbers.setValues(-3, 8);
    cout << "After change:\n";
    numbers.print();
    cout << "Sum: " << numbers.sum() << '\n';
    cout << "Maximum: " << numbers.maximum() << '\n';
    return 0;
}
