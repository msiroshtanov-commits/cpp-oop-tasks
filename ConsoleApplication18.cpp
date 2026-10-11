#include <iostream>
#include <string>
#include <utility>
using namespace std;
const int N = 13;
bool inputArray(string a[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << "A[" << i + 1 << "]: ";
        if (!getline(cin, a[i]))
            return false;
    }
    return true;
}
void printArray(const string a[], int n) {
    for (int i = 0; i < n; ++i)
        cout << i + 1 << ": " << a[i] << '\n';
}
void reverseArray(string a[], int n) {
    for (int i = 0; i < n / 2; ++i)
        swap(a[i], a[n - 1 - i]);
}
int main() {
    string a[N];
    cout << "Enter 13 strings, one per line.\n";
    if (!inputArray(a, N)) {
        cerr << "Input error.\n";
        return 1;
    }
    cout << "\nOriginal array:\n";
    printArray(a, N);
    reverseArray(a, N);
    cout << "\nReversed array:\n";
    printArray(a, N);
    return 0;
}
