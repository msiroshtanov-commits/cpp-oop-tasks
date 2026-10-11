#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;
const int STUDENTS = 10;
const int SUBJECTS = 4;
struct Student {
    string name;
    int grades[SUBJECTS];
};
bool inputStudents(Student a[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << "Student " << i + 1 << " name: ";
        if (!getline(cin, a[i].name) || a[i].name.empty())
            return false;
        cout << "Grades: informatics, math, physics, programming: ";
        for (int j = 0; j < SUBJECTS; ++j) {
            if (!(cin >> a[i].grades[j])
                || a[i].grades[j] < 2 || a[i].grades[j] > 5)
                return false;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return true;
}
double averageGrade(const Student& s) {
    int sum = 0;
    for (int j = 0; j < SUBJECTS; ++j)
        sum += s.grades[j];
    return static_cast<double>(sum) / SUBJECTS;
}
void printStudents(const Student a[], int n) {
    cout << "\nNo | Name | Inf | Math | Phys | Prog | Average\n";
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; ++i) {
        cout << i + 1 << " | " << a[i].name;
        for (int j = 0; j < SUBJECTS; ++j)
            cout << " | " << a[i].grades[j];
        cout << " | " << averageGrade(a[i]) << '\n';
    }
}
int main() {
    Student students[STUDENTS];
    cout << "Enter data for 10 students.\n";
    if (!inputStudents(students, STUDENTS)) {
        cerr << "Invalid name or grades. Use grades from 2 to 5.\n";
        return 1;
    }
    printStudents(students, STUDENTS);
    return 0;
}
