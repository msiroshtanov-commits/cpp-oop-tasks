#include <iostream>
using namespace std;

int main() {
    int sum = 0;

    for (int i = 1; i <= 1000; ++i)
        sum += i;

    cout << "Sum from 1 to 1000: "
        << sum << endl;
    return 0;
}