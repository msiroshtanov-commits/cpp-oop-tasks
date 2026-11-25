// Тема 15. Сіроштанов Максим Володимирович, Ік-52.
// Об'єктно-орієнтоване програмування. Приклади для AR Viewer.
#include <clocale>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#endif

class Building {
    std::wstring name_;
    int floors_;

public:
    Building(const std::wstring& name, int floors)
        : name_(name), floors_(floors) {}

    virtual ~Building() = default;

    virtual void print() const {
        std::wcout << L"Будинок: " << name_
                   << L", поверхів: " << floors_ << L".\n";
    }

protected:
    const std::wstring& name() const { return name_; }
    int floors() const { return floors_; }
};

class AcademicBuilding : public Building {
    std::wstring arRoom_;

public:
    AcademicBuilding(const std::wstring& name, int floors,
                     const std::wstring& arRoom)
        : Building(name, floors), arRoom_(arRoom) {}

    void print() const override {
        std::wcout << L"Навчальний корпус: " << name()
                   << L", поверхів: " << floors()
                   << L", аудиторія для показу AR: " << arRoom_ << L".\n";
    }
};

class DigitalDevice {
    std::wstring model_;

public:
    explicit DigitalDevice(const std::wstring& model) : model_(model) {}

    virtual ~DigitalDevice() = default;

    virtual void print() const {
        std::wcout << L"Цифровий пристрій: " << model_ << L".\n";
    }

protected:
    const std::wstring& model() const { return model_; }
};

class MobilePhone : public DigitalDevice {
    std::wstring operatingSystem_;
    bool hasCamera_;

public:
    MobilePhone(const std::wstring& model, const std::wstring& operatingSystem,
                bool hasCamera)
        : DigitalDevice(model), operatingSystem_(operatingSystem),
          hasCamera_(hasCamera) {}

    void print() const override {
        std::wcout << L"Мобільний телефон: " << model()
                   << L", ОС: " << operatingSystem_
                   << L", камера: " << (hasCamera_ ? L"є" : L"немає") << L".\n";
    }
};

// N визначається з масиву. Обидві межі включено.
template <typename T, std::size_t N>
std::vector<T> findInRange(const T (&values)[N], T low, T high) {
    std::vector<T> matches;
    for (const T& value : values) {
        if (low <= value && value <= high) {
            matches.push_back(value);
        }
    }
    return matches;
}

template <typename T>
void printValues(const std::vector<T>& values) {
    if (values.empty()) {
        std::wcout << L"збігів немає";
        return;
    }
    const wchar_t* separator = L"";
    for (const T& value : values) {
        std::wcout << separator << value;
        separator = L", ";
    }
}

template <typename First, typename Second>
class NumberPair {
    First first_;
    Second second_;

public:
    NumberPair(First first, Second second) : first_(first), second_(second) {}

    First first() const { return first_; }
    Second second() const { return second_; }
};

int main() {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
#else
    std::setlocale(LC_ALL, "");
#endif

    std::wcout << L"Тема 15 | AR Viewer | Сіроштанов Максим, Ік-52\n\n";

    std::wcout << L"1. Будинок та навчальний корпус\n";
    const Building house(L"Гуртожиток", 5);
    const AcademicBuilding academicBuilding(L"Корпус програмної інженерії",
                                            4, L"204");
    const Building* buildings[] = {&house, &academicBuilding};
    for (const Building* building : buildings) {
        building->print();
    }

    std::wcout << L"\n2. Цифровий пристрій та мобільний телефон\n";
    const DigitalDevice computer(L"ПК для підготовки 3D-моделей");
    const MobilePhone phone(L"Samsung Galaxy A34", L"Android", true);
    const DigitalDevice* devices[] = {&computer, &phone};
    for (const DigitalDevice* device : devices) {
        device->print();
    }

    std::wcout << L"\n3. Пошук елементів у діапазоні\n";
    const int triangleCounts[] = {120, 500, 800, 1000, 1500};
    const double scales[] = {0.40, 0.50, 1.00, 1.50, 2.00, 2.30};
    std::wcout << L"Кількість трикутників [500; 1000] (int): ";
    printValues(findInRange(triangleCounts, 500, 1000));
    std::wcout << std::fixed << std::setprecision(2)
               << L"\nМасштаб [0.50; 2.00] (double): ";
    printValues(findInRange(scales, 0.50, 2.00));
    std::wcout << L"\nКількість трикутників [1600; 2000]: ";
    printValues(findInRange(triangleCounts, 1600, 2000));

    std::wcout << L"\n\n4. Пари чисел різних типів\n";
    const NumberPair<int, double> modelAndScale(2, 1.25);
    const NumberPair<double, int> angleAndTriangles(45.50, 800);
    std::wcout << L"(int, double) ID моделі та масштаб: ("
               << modelAndScale.first() << L"; " << modelAndScale.second() << L")\n";
    std::wcout << L"(double, int) Кут (градуси) та кількість трикутників: ("
               << angleAndTriangles.first() << L"; " << angleAndTriangles.second()
               << L")\n";

    return 0;
}
