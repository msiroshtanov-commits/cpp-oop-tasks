#include <iostream>
using namespace std;

int main() {
    double num;
    cout << "Enter a number: ";
    cin >> num;

    if (num < 10)
        cout << "Less than 10" << endl;
    else if (num == 10)
        cout << "Equal to 10" << endl;
    else
        cout << "Greater than 10" << endl;

    return 0;
}