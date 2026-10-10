#include <iostream>
using namespace std;

int main() {
    double B[6] = { 21.3, 30.5, -6.8, 0.3, -1.2, 5.3 };
    double minElement = B[0];

    for (int i = 1; i < 6; i++) {
        if (B[i] < minElement) {
            minElement = B[i];
        }
    }

    cout << "Minimalnyi element: " << minElement << endl;
    return 0;
}
