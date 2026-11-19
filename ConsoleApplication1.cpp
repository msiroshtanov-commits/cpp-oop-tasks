// Тема 14. Методи та асоціації класів. Вправи 5.4, 5.5, 5.11.
// Сіроштанов Максим Володимирович, Ік-52.
// Консольний застосунок C++17. У Visual Studio: параметр /utf-8.
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

class Employer;
class Employee;
class Person;

class Accountant {
    friend class Employer;
    friend class Position;
public:
    ~Accountant() = default;
    void payroll() const;
private:
    Accountant(const Employer&, const Person&);
    void payroll(const Employee&) const;
    const Employer& _myEmployer;
    const Person& _who;
    Accountant(const Accountant&) = delete;
    Accountant& operator=(const Accountant&) = delete;
};

class Person {
public:
    explicit Person(const char* name) : _name(name) {}
    const std::string& getName() const { return _name; }
    Person(const Person&) = delete;
    Person& operator=(const Person&) = delete;
private:
    std::string _name;
};

class Position {
    friend void Accountant::payroll(const Employee&) const;
public:
    Position(const char* name, unsigned int salary)
        : _name(name), _salary(salary) {
    }
    const std::string& getPositionName() const { return _name; }
    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;
private:
    std::string _name;
    unsigned int _salary;
    unsigned int getSalary() const { return _salary; }
};

class Employee {
    friend class Employer;
public:
    const Person& who() const { return _who; }
    const Position& what() const { return _what; }
private:
    Employee(const Person& who, const Position& what)
        : _what(what), _who(who) {
    }
    ~Employee() = default;
    const Position& _what;
    const Person& _who;
    Employee(const Employee&) = delete;
    Employee& operator=(const Employee&) = delete;
};

class Employer {
    friend void Accountant::payroll() const;
public:
    Employer(const Person&, std::size_t, const Person&);
    ~Employer();
    std::size_t volume() const { return _volume; }
    void hire(const Person&, const Position&);
    void fire(const std::size_t i);
    void pay() const { _accountant->payroll(); }
    Employer(const Employer&) = delete;
    Employer& operator=(const Employer&) = delete;
private:
    struct Staff {
        Employee* _pemployee = nullptr;
        bool _free = true;
    };
    std::size_t _volume;
    std::unique_ptr<Staff[]> _office;
    std::unique_ptr<Accountant> _accountant;
    const Person& _me;
};

Accountant::Accountant(const Employer& employer, const Person& who)
    : _myEmployer(employer), _who(who) {
}

Employer::Employer(const Person& boss, std::size_t volume,
    const Person& accountant)
    : _volume(volume), _office(std::make_unique<Staff[]>(volume)),
    _accountant(new Accountant(*this, accountant)), _me(boss) {
}

Employer::~Employer() {
    for (std::size_t i = 0; i < _volume; ++i) {
        delete _office[i]._pemployee;
    }
    // Масив Staff і бухгалтер видаляються через unique_ptr.
}

// Вправа 5.4. Спочатку шукаємо вакансію, потім створюємо Employee.
void Employer::hire(const Person& person, const Position& position) {
    std::size_t i = 0;
    while (i < _volume && !_office[i]._free) {
        ++i;
    }
    if (i == _volume) {
        throw std::runtime_error("Немає вакантних посад.");
    }
    Employee* employee = new Employee(person, position);
    _office[i]._pemployee = employee;
    _office[i]._free = false;
}

// Вправа 5.5. Нумерація робочих місць починається з нуля.
void Employer::fire(const std::size_t i) {
    if (i >= _volume) {
        throw std::out_of_range("Неправильний індекс робочого місця.");
    }
    if (_office[i]._free) {
        throw std::logic_error("Це робоче місце вже вільне.");
    }
    delete _office[i]._pemployee;
    _office[i]._pemployee = nullptr;
    _office[i]._free = true;
}

void Accountant::payroll(const Employee& employee) const {
    std::cout << employee.who().getName() << ": "
        << employee.what().getPositionName() << ", "
        << employee.what().getSalary() << " грн\n";
}

void Accountant::payroll() const {
    std::cout << "Нарахування зарплати (бухгалтер: "
        << _who.getName() << "):\n";
    for (std::size_t i = 0; i < _myEmployer.volume(); ++i) {
        if (!_myEmployer._office[i]._free) {
            payroll(*_myEmployer._office[i]._pemployee);
        }
    }
}

// Вправа 5.11. Усі операції змінюють регістр акумулятора _a.
struct Calc {
    double _a = 0.0;
    double _x = 0.0;
    double add() { return _a += _x; }
    double sub() { return _a -= _x; }
    double mul() { return _a *= _x; }
    double div() {
        if (_x == 0.0) {
            throw std::domain_error("Ділення на нуль заборонено.");
        }
        return _a /= _x;
    }
};

// operation - указівник на метод Calc без параметрів із результатом double.
double exe(Calc& calc, double (Calc::* operation)()) {
    if (operation == nullptr) {
        throw std::invalid_argument("Операцію не вибрано.");
    }
    return (calc.*operation)();
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "Тема 14. Сіроштанов Максим, Ік-52\n\n";
    // Person і Position мають існувати довше, ніж Employer.
    const Person boss("Директор"), accountant("Олена");
    const Person anna("Анна"), bohdan("Богдан"), maksym("Максим");
    const Position developer("Розробник", 30000);
    const Position tester("Тестувальник", 25000);
    Employer company(boss, 2, accountant);
    company.hire(anna, developer);
    company.hire(bohdan, tester);
    company.pay();

    try {
        company.hire(maksym, developer);
    }
    catch (const std::exception& error) {
        std::cout << "Спроба наймання: " << error.what() << '\n';
    }
    company.fire(0);
    std::cout << "Місце 0 звільнено.\n";
    try {
        company.fire(0);
    }
    catch (const std::exception& error) {
        std::cout << "Повторне звільнення: " << error.what() << '\n';
    }
    try {
        company.fire(5);
    }
    catch (const std::exception& error) {
        std::cout << "Звільнення місця 5: " << error.what() << '\n';
    }
    company.hire(maksym, developer);
    company.pay();

    std::cout << "\nКалькулятор:\n" << std::fixed << std::setprecision(2);
    Calc calc{ 10.0, 2.0 };
    std::cout << "10 + 2 = " << exe(calc, &Calc::add) << '\n';
    calc._x = 4.0;
    std::cout << "12 - 4 = " << exe(calc, &Calc::sub) << '\n';
    calc._x = 2.0;
    std::cout << "8 * 2 = " << exe(calc, &Calc::mul) << '\n';
    calc._x = 4.0;
    std::cout << "16 / 4 = " << exe(calc, &Calc::div) << '\n';
    calc._x = 0.0;
    try {
        exe(calc, &Calc::div);
    }
    catch (const std::exception& error) {
        std::cout << error.what() << " Акумулятор = " << calc._a << '\n';
    }
    return 0;
}