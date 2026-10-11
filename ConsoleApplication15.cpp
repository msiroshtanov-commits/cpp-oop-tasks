#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <iomanip>
using namespace std;
class Student {
    string fullName, group;
    array<int, 5> grades;
public:
    Student(string name, string g, array<int, 5> marks)
        : fullName(name), group(g), grades(marks) {
    }
    double average() const {
        int sum = 0;
        for (int mark : grades) sum += mark;
        return sum / 5.0;
    }
    bool onlyFourOrFive() const {
        for (int mark : grades)
            if (mark != 4 && mark != 5) return false;
        return true;
    }
    void print() const {
        cout << fullName << " | " << group << " | "
            << fixed << setprecision(2) << average() << '\n';
    }
    void printNameAndGroup() const {
        cout << fullName << " | " << group << '\n';
    }
};
int main() {
    Student students[] = {
        {"Siroshtanov M. V.", "Ik-52", {5, 4, 5, 4, 5}},
        {"Petrenko A. I.", "Ik-52", {3, 4, 3, 4, 3}},
        {"Kovalenko O. P.", "Ik-51", {5, 5, 5, 5, 5}},
        {"Melnyk D. S.", "Ik-52", {4, 4, 4, 4, 4}},
        {"Bondarenko I. M.", "Ik-51", {5, 5, 3, 5, 5}}
    };
    int n = sizeof(students) / sizeof(students[0]);
    stable_sort(students, students + n,
        [](const Student& a, const Student& b) {
            return a.average() < b.average();
        });
    cout << "Sorted by average:\n";
    for (const Student& s : students) s.print();
    cout << "Only grades 4 or 5:\n";
    for (const Student& s : students)
        if (s.onlyFourOrFive()) s.printNameAndGroup();
    return 0;
}
