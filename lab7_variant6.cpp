#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <iomanip>
#include <cmath>
#include <stdexcept>
using namespace std;
class Curve {
private:
    string name;
public:
    Curve() : name("Unnamed") {}
    explicit Curve(const string& n) : name(n) {}
    virtual ~Curve() = default;
    const string& getName() const { return name; }
    virtual int type() const = 0;
    virtual double value(double x) const = 0;
    virtual void print() const = 0;
};
class Line : public Curve {
private:
    double a, b;
public:
    Line() : Curve(), a(0), b(0) {}
    Line(const string& n, double ca, double cb)
        : Curve(n), a(ca), b(cb) {
    }
    int type() const override { return 1; }
    double value(double x) const override {
        return a * x + b;
    }
    void print() const override {
        cout << getName() << " | Line | a=" << a;
        cout << " b=" << b << " | y=a*x+b\n";
    }
};
class Parabola : public Curve {
private:
    double a, b;
public:
    Parabola() : Curve(), a(0), b(0) {}
    Parabola(const string& n, double ca, double cb)
        : Curve(n), a(ca), b(cb) {
    }
    int type() const override { return 2; }
    double value(double x) const override {
        return a * x * x + b;
    }
    void print() const override {
        cout << getName() << " | Parabola | a=" << a;
        cout << " b=" << b << " | y=a*x^2+b\n";
    }
};
// The lab calls this Hyperbola but specifies a*x^3+b.
class Hyperbola : public Curve {
private:
    double a, b;
public:
    Hyperbola() : Curve(), a(0), b(0) {}
    Hyperbola(const string& n, double ca, double cb)
        : Curve(n), a(ca), b(cb) {
    }
    int type() const override { return 3; }
    double value(double x) const override {
        return a * x * x * x + b;
    }
    void print() const override {
        cout << getName() << " | Hyperbola (lab formula) | a=" << a;
        cout << " b=" << b << " | y=a*x^3+b\n";
    }
};
int readInt(const char* prompt) {
    cout << prompt;
    int v;
    if (!(cin >> v))
        throw runtime_error("Expected an integer.");
    return v;
}
double readNumber(const char* prompt) {
    cout << prompt;
    double v;
    if (!(cin >> v) || !isfinite(v))
        throw runtime_error("Expected a finite number.");
    return v;
}
unique_ptr<Curve> inputCurve() {
    int t = readInt("Type: 1 Line, 2 Parabola, 3 Hyperbola: ");
    if (t < 1 || t > 3)
        throw runtime_error("Type must be from 1 to 3.");
    cout << "Record name: ";
    string name;
    if (!getline(cin >> ws, name) || name.empty())
        throw runtime_error("Expected a record name.");
    double a = readNumber("a: ");
    double b = readNumber("b: ");
    if (t == 1)
        return unique_ptr<Curve>(new Line(name, a, b));
    if (t == 2)
        return unique_ptr<Curve>(new Parabola(name, a, b));
    return unique_ptr<Curve>(new Hyperbola(name, a, b));
}
void showRecords(const vector<unique_ptr<Curve>>& records) {
    if (records.empty()) cout << "No records.\n";
    for (size_t i = 0; i < records.size(); ++i) {
        cout << i + 1 << " | ";
        records[i]->print();
    }
}
void searchRecords(const vector<unique_ptr<Curve>>& records) {
    int t = readInt("Search type (1-3): ");
    if (t < 1 || t > 3) {
        cout << "Invalid type.\n";
        return;
    }
    bool found = false;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i]->type() == t) {
            cout << i + 1 << " | ";
            records[i]->print();
            found = true;
        }
    }
    if (!found) cout << "No matching records.\n";
}
void editRecord(vector<unique_ptr<Curve>>& records) {
    int n = readInt("Record number to edit: ");
    if (n < 1 || static_cast<size_t>(n) > records.size()) {
        cout << "Invalid record number.\n";
        return;
    }
    unique_ptr<Curve> replacement = inputCurve();
    records[n - 1] = move(replacement);
    cout << "Record updated.\n";
}
void evaluateRecords(const vector<unique_ptr<Curve>>& records) {
    if (records.empty()) {
        cout << "No records.\n";
        return;
    }
    double x = readNumber("x: ");
    for (size_t i = 0; i < records.size(); ++i) {
        double y = records[i]->value(x);
        cout << i + 1 << " | " << records[i]->getName();
        if (isfinite(y)) cout << " | y=" << y << '\n';
        else cout << " | Result out of range.\n";
    }
}
int main() {
    vector<unique_ptr<Curve>> records;
    cout << fixed << setprecision(2);
    cout << "Default constructor examples:\n";
    {
        Line line;
        Parabola parabola;
        Hyperbola hyperbola;
        line.print();
        parabola.print();
        hyperbola.print();
    }
    try {
        while (true) {
            cout << "\n1 Add, 2 Show, 3 Search by type\n";
            cout << "4 Edit, 5 Calculate y(x), 0 Exit\n";
            int command = readInt("Choice: ");
            if (command == 0) break;
            switch (command) {
            case 1:
                records.push_back(inputCurve());
                cout << "Record added.\n";
                break;
            case 2: showRecords(records); break;
            case 3: searchRecords(records); break;
            case 4: editRecord(records); break;
            case 5: evaluateRecords(records); break;
            default: cout << "Invalid command.\n";
            }
        }
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    cout << "Program finished.\n";
    return 0;
}
