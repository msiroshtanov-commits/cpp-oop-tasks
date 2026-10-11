#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;
struct Notebook {
    string manufacturer;
    double price;
    double diagonal;
};
int main() {
    const int MAX = 100;
    Notebook notebooks[MAX], loaded[MAX];
    int n;
    cout << "Number of notebooks (1-100): ";
    if (!(cin >> n) || n < 1 || n > MAX) {
        cerr << "Invalid number of records.\n";
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        cout << "Notebook " << i + 1 << "\nManufacturer: ";
        if (!getline(cin >> ws, notebooks[i].manufacturer))
            return 1;
        cout << "Price (UAH) and diagonal (inches): ";
        if (!(cin >> notebooks[i].price >> notebooks[i].diagonal)
            || !isfinite(notebooks[i].price)
            || !isfinite(notebooks[i].diagonal)
            || notebooks[i].price < 0 || notebooks[i].diagonal <= 0) {
            cerr << "Invalid price or diagonal.\n";
            return 1;
        }
    }
    ofstream out("notebooks.txt");
    if (!out) {
        cerr << "Cannot open file for writing.\n";
        return 1;
    }
    out << n << '\n' << setprecision(17);
    for (int i = 0; i < n; ++i)
        out << notebooks[i].manufacturer << '\n'
        << notebooks[i].price << ' '
        << notebooks[i].diagonal << '\n';
    out.close();
    if (!out) {
        cerr << "File write error.\n";
        return 1;
    }
    ifstream in("notebooks.txt");
    int count;
    if (!(in >> count) || count < 1 || count > MAX) {
        cerr << "Cannot read file or invalid record count.\n";
        return 1;
    }
    for (int i = 0; i < count; ++i) {
        if (!getline(in >> ws, loaded[i].manufacturer)
            || !(in >> loaded[i].price >> loaded[i].diagonal)
            || !isfinite(loaded[i].price)
            || !isfinite(loaded[i].diagonal)
            || loaded[i].price < 0 || loaded[i].diagonal <= 0) {
            cerr << "Invalid data in file.\n";
            return 1;
        }
    }
    in.close();
    cout << "\nFile notebooks.txt saved and read.\n";
    cout << "Notebooks priced over 25000 UAH:\n";
    cout << "Manufacturer | Price (UAH) | Diagonal (inches)\n";
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (loaded[i].price > 25000) {
            cout << loaded[i].manufacturer << " | "
                << fixed << setprecision(2) << loaded[i].price
                << " | " << loaded[i].diagonal << '\n';
            found = true;
        }
    }
    if (!found) cout << "No matching notebooks.\n";
    return 0;
}
