#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
using namespace std;

class Circle {
private:
    double x, y, radius;

    static void validate(double cx, double cy, double r) {
        if (!isfinite(cx) || !isfinite(cy)
            || !isfinite(r) || r < 0)
            throw invalid_argument("Invalid center or radius.");
    }

public:
    Circle() : x(0), y(0), radius(0) {}

    Circle(double cx, double cy, double r)
        : x(cx), y(cy), radius(r) {
        validate(x, y, radius);
    }

    ~Circle() {
        cout << "Circle destroyed, radius = " << radius << '\n';
    }

    bool input() {
        double cx, cy, r;
        cout << "Enter center x, center y and radius: ";
        if (!(cin >> cx >> cy >> r))
            return false;
        validate(cx, cy, r);
        x = cx;
        y = cy;
        radius = r;
        return true;
    }

    double getX() const { return x; }
    double getY() const { return y; }
    double getRadius() const { return radius; }

    double area() const {
        const double pi = acos(-1.0);
        return pi * radius * radius;
    }

    double circumference() const {
        const double pi = acos(-1.0);
        return 2 * pi * radius;
    }

    void print() const {
        cout << fixed << setprecision(2);
        cout << "Center: (" << x << ", " << y << ")\n";
        cout << "Radius: " << radius << '\n';
        cout << "Area: " << area() << '\n';
        cout << "Circumference: " << circumference() << '\n';
    }

    Circle operator*(double k) const {
        if (!isfinite(k) || k < 0)
            throw invalid_argument("Multiplier must be >= 0.");
        return Circle(x, y, radius * k);
    }

    Circle operator/(double k) const {
        if (!isfinite(k) || k <= 0)
            throw invalid_argument("Divisor must be > 0.");
        return Circle(x, y, radius / k);
    }
};

int main() {
    try {
        Circle initial;
        cout << "Default object:\n";
        initial.print();
        if (!initial.input()) {
            cerr << "Input error. Use decimal points.\n";
            return 1;
        }
        Circle circle(initial.getX(), initial.getY(),
            initial.getRadius());
        cout << "\nObject created with parameters:\n";
        circle.print();
        double multiplier, divisor;
        cout << "Enter multiplier and divisor: ";
        if (!(cin >> multiplier >> divisor)) {
            cerr << "Input error.\n";
            return 1;
        }
        Circle multiplied = circle * multiplier;
        cout << "\nAfter multiplication:\n";
        multiplied.print();
        Circle divided = circle / divisor;
        cout << "\nAfter division:\n";
        divided.print();
        cout << "\nOriginal circle:\n";
        circle.print();
        cout << "\nLeaving scope:\n";
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
