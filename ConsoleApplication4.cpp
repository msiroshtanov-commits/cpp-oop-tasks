#include <iostream>
using namespace std;
int main() {
    double B[6];
    cout << "Enter 6 numbers: " << endl;
    for (int i = 0; i < 6; ++i)
        if (!(cin >> B[i])) return 1;
    double minElement = B[0];
    for (int i = 1; i < 6; ++i) {
        if (B[i] < minElement)
            minElement = B[i];
    }
    cout << "Minimum: " << minElement << endl;
    return 0;
}
