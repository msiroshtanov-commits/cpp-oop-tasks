#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
int main() {
    int m, n; cout << "Enter m and n: " << endl;
    if (!(cin >> m >> n)) return 1;
    if (m < 1 || m > 5 || n < 1 || n > 4) {
        cout << "Invalid dimensions" << endl; return 1;
    }
    int A[5][4], values[20], k = 0;
    cout << "Enter matrix elements: " << endl;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            if (!(cin >> A[i][j])) return 1;
            values[k++] = A[i][j];
        }
    sort(values, values + k, greater<int>());
    k = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            A[i][j] = values[k++];
    cout << "Sorted matrix:" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            cout << A[i][j] << '\t';
        cout << endl;
    }
    return 0;
}
