#include <iostream>
#include <stdexcept>
using namespace std;
class DecimalCounter {
    int current, lower, upper;
public:
    DecimalCounter() : DecimalCounter(0, 0, 9) {}
    DecimalCounter(int value, int minValue, int maxValue)
        : current(value), lower(minValue), upper(maxValue) {
        if (lower > upper || current < lower || current > upper)
            throw invalid_argument("Invalid counter values");
    }
    void increment() {
        if (current < upper) ++current;
    }
    void decrement() {
        if (current > lower) --current;
    }
    int getCurrent() const { return current; }
};
int main() {
    DecimalCounter standard;
    cout << "Default: " << standard.getCurrent() << '\n';
    standard.increment();
    cout << "Increment: " << standard.getCurrent() << '\n';
    standard.decrement();
    cout << "Decrement: " << standard.getCurrent() << '\n';
    standard.decrement();
    cout << "Lower limit: " << standard.getCurrent() << '\n';
    DecimalCounter custom(5, 3, 7);
    cout << "Custom: " << custom.getCurrent() << '\n';
    cout << "Increment sequence: ";
    for (int i = 0; i < 3; ++i) {
        custom.increment();
        cout << custom.getCurrent() << ' ';
    }
    cout << "\nDecrement sequence: ";
    for (int i = 0; i < 5; ++i) {
        custom.decrement();
        cout << custom.getCurrent() << ' ';
    }
    cout << '\n';
    try {
        DecimalCounter invalid(10, 0, 9);
    }
    catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << '\n';
    }
    return 0;
}
