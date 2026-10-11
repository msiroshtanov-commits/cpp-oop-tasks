#include <iostream>
#include <string>
using namespace std;
 
int main() {
    string users[5];
    users[0] = "Maksym";
    users[1] = "Roma";
    users[2] = "Vlad";
    users[3] = "Igor";
    users[4] = "Anna";
    cout << "Task 1: ";
    for (const string& name : users)
        cout << name << " ";
    int numbers[] = {1, 15, 2};
    int n = sizeof(numbers) / sizeof(numbers[0]);
    int sum = 0;
    cout << "\nTask 2: ";
    for (int i = 0; i < n; ++i) {
        cout << numbers[i] << " ";
        sum += numbers[i];
    }
    cout << "\nTask 3: " << sum;
    cout << "\nTask 4: " << n << "\n";
    return 0;
}
