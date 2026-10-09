#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));
    int a[4];
    bool increasing = true;

    for (int i = 0; i < 4; i++) {
        a[i] = 10 + rand() % 90;
        cout << a[i] << " ";
    }

    for (int i = 1; i < 4; i++) {
        if (a[i] <= a[i - 1]) {
            increasing = false;
            break;
        }
    }

    cout << endl;
    if (increasing)
        cout << "Масив є строго зростаючою послідовністю." << endl;
    else
        cout << "Масив не є строго зростаючою послідовністю." << endl;

    return 0;
}