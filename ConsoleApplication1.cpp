#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// One-dimensional profiles for a future AR geometry module.
class ElementaryFunction {
    int id_;
public:
    explicit ElementaryFunction(int id) : id_(id) {}
    static void printMenu() {
        std::cout << "1. x^2\n2. sin(x)\n3. cos(x)\n"
            "4. exp(x)\n5. ln(x)\n6. sqrt(x)\n";
    }
    void validateInterval(double a, double b) const {
        if (id_ == 5 && a <= 0)
            throw std::domain_error("ln(x) requires a > 0.");
        if (id_ == 6 && a < 0)
            throw std::domain_error("sqrt(x) requires a >= 0.");
        (void)value(a);
        (void)value(b);
    }
    double value(double x) const {
        double y = 0;
        switch (id_) {
        case 1: y = x * x; break;
        case 2: y = std::sin(x); break;
        case 3: y = std::cos(x); break;
        case 4: y = std::exp(x); break;
        case 5: y = std::log(x); break;
        case 6: y = std::sqrt(x); break;
        default: throw std::invalid_argument("Unknown function.");
        }
        if (!std::isfinite(y))
            throw std::overflow_error("Function value is not finite.");
        return y;
    }
};

class NumericalMethods {
public:
    static void tabulate(const ElementaryFunction& f,
        double a, double b, int n) {
        const double h = (b - a) / n;
        std::vector<double> values;
        for (int i = 0; i <= n; ++i) {
            const double x = (i == n) ? b : a + i * h;
            values.push_back(f.value(x));
        }
        std::cout << "Step h = " << h << '\n'
            << std::setw(16) << "x"
            << std::setw(20) << "f(x)" << '\n';
        for (int i = 0; i <= n; ++i) {
            const double x = (i == n) ? b : a + i * h;
            std::cout << std::setw(16) << x
                << std::setw(20) << values[i] << '\n';
        }
    }
    static double simpson(const ElementaryFunction& f,
        double a, double b, int n) {
        if (n < 2 || n % 2 != 0)
            throw std::invalid_argument("Simpson requires even n >= 2.");
        const double h = (b - a) / n;
        long double sum = static_cast<long double>(f.value(a))
            + f.value(b);
        for (int i = 1; i < n; ++i) {
            const double x = a + i * h;
            const int weight = (i % 2 == 0) ? 2 : 4;
            sum += weight * static_cast<long double>(f.value(x));
        }
        const double result = static_cast<double>(sum * h / 3.0L);
        if (!std::isfinite(result))
            throw std::overflow_error("Integral result is not finite.");
        return result;
    }
};

class Application {
    template<class T>
    T read(const std::string& prompt, T low, T high) const {
        for (;;) {
            std::cout << prompt;
            std::string line;
            if (!std::getline(std::cin, line))
                throw std::runtime_error("Input closed.");
            std::istringstream stream(line);
            T value{};
            if (stream >> value) {
                stream >> std::ws;
                if (stream.eof() && std::isfinite(static_cast<double>(value))
                    && value >= low && value <= high)
                    return value;
            }
            std::cout << "Invalid input. Allowed range: "
                << low << " to " << high << ".\n";
        }
    }
    void handleCalculation(bool integral) const {
        ElementaryFunction::printMenu();
        const int id = read<int>("Function (1-6): ", 1, 6);
        const ElementaryFunction f(id);
        const double a = read<double>("a: ", -1000000.0, 1000000.0);
        const double b = read<double>("b: ", -1000000.0, 1000000.0);
        if (a >= b)
            throw std::invalid_argument("Interval requires a < b.");
        f.validateInterval(a, b);
        int n = 0;
        if (integral) {
            do {
                n = read<int>("Even subdivisions n (2-100000): ",
                    2, 100000);
                if (n % 2 != 0)
                    std::cout << "n must be even. Try again.\n";
            } while (n % 2 != 0);
        }
        else {
            n = read<int>("Subdivisions n (1-1000): ", 1, 1000);
        }
        const double h = (b - a) / n;
        if (!(h > 0) || a + h == a || b - h == b)
            throw std::invalid_argument("Step is too small for this interval.");
        std::cout << std::fixed << std::setprecision(6);
        if (integral)
            std::cout << "Simpson integral = "
            << NumericalMethods::simpson(f, a, b, n) << '\n';
        else
            NumericalMethods::tabulate(f, a, b, n);
    }
    void handleTabulation() const { handleCalculation(false); }
    void handleIntegration() const { handleCalculation(true); }
public:
    void run() const {
        std::cout << "AR profile calculations - Topic 10\n"
            "Trigonometric arguments are in radians.\n"
            "Use a decimal point; enter each value on its own line.\n";
        for (;;) {
            try {
                std::cout << "\n1. Tabulate a function\n"
                    "2. Integrate using Simpson\n0. Exit\n";
                const int command = read<int>("Command: ", 0, 2);
                switch (command) {
                case 1: handleTabulation(); break;
                case 2: handleIntegration(); break;
                case 0: std::cout << "Goodbye!\n"; return;
                }
            }
            catch (const std::exception& error) {
                std::cout << "Error: " << error.what() << '\n';
                if (std::cin.eof()) return;
            }
        }
    }
};

int main() {
    const Application application;
    application.run();
}
