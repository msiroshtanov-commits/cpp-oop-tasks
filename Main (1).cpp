#include <iostream>
using namespace std;
 
int main() {
    int a[] = {0, 1, 3, 4, 5, 2, 1, -4, -1, 10, 55};
    int n = sizeof(a) / sizeof(a[0]);
    cout << "Block 1: ";
    for (int i = 0; i < n; ++i)
        cout << a[i] << " ";
    cout << "\nBlock 2: ";
    for (int i = n - 1; i >= 0; --i)
        cout << a[i] << " ";
    cout << "\nBlock 3: ";
    for (int i = 0; i < n; ++i)
        if (a[i] % 2 == 0)
            cout << a[i] << " ";
    cout << "\nBlock 4: ";
    for (int i = 0; i < n; i += 2)
        cout << a[i] << " ";
    cout << "\nBlock 5: ";
    for (int i = 0; i < n; ++i) {
        if (a[i] == -1) break;
        cout << a[i] << " ";
    }
    cout << "\n";
    return 0;
}

