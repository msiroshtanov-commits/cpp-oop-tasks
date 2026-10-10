#include <iostream>
using namespace std;

int main() {
    int i = 0, sum = 0;

    while (i < 1000) {
        ++i;
        sum += i;
    }

    cout << "Sum from 1 to 1000: "
        << sum << endl;
    return 0;
}