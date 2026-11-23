// Практичне заняття №11. Сіроштанов Максим Володимирович, Ік-52.
#include <charconv>
#include <cmath>
#include <cwctype>
#include <iomanip>
#include <initializer_list>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace oop11 {
class Error {
public:
    std::wstring message;
    explicit Error(std::wstring text) : message(std::move(text)) {}
};

double checked(double value) {
    if (!std::isfinite(value)) throw Error(L"Результат виходить за межі типу double.");
    return value;
}

std::wstring literal(double value) {
    char buffer[64];
    auto res = std::to_chars(buffer, buffer + sizeof(buffer), value);
    if (res.ec != std::errc{}) throw Error(L"Не вдалося відобразити число.");
    return std::wstring(buffer, res.ptr);
}

class Telement;
using Tree = std::unique_ptr<Telement>;

class Telement {
protected:
    Tree left, right;

    explicit Telement(Tree l = {}, Tree r = {})
        : left(std::move(l)), right(std::move(r)),
          count_(1 + (left ? left->count_ : 0) + (right ? right->count_ : 0)) {
        if (count_ > 24000) throw Error(L"Дерево надто велике. Спростіть вираз або зменште порядок.");
    }

    std::wstring binary(wchar_t op) const {
        return L"(" + left->text() + L" " + op + L" " + right->text() + L")";
    }

public:
    virtual ~Telement() = default;
    virtual double rezult() const = 0;
    virtual Tree copy() const = 0;
    virtual Tree differ() const = 0; // Завжди диференціюємо за x.
    virtual std::wstring text() const = 0;
    virtual void set_var(double x, double y = 0) {
        if (left) left->set_var(x, y);
        if (right) right->set_var(x, y);
    }

private:
    unsigned count_;
};

class Real final : public Telement {
    double value_;
public:
    explicit Real(double value) : value_(value) {}
    double rezult() const override { return value_; }
    Tree copy() const override { return std::make_unique<Real>(value_); }
    Tree differ() const override { return std::make_unique<Real>(0); }
    std::wstring text() const override { return literal(value_); }
};

class Var final : public Telement {
    wchar_t name_;
    double value_;
public:
    explicit Var(wchar_t name, double value = 0) : name_(name), value_(value) {}
    double rezult() const override { return value_; }
    Tree copy() const override { return std::make_unique<Var>(name_, value_); }
    Tree differ() const override { return std::make_unique<Real>(name_ == L'x' ? 1 : 0); }
    std::wstring text() const override { return std::wstring(1, name_); }
    void set_var(double x, double y) override { value_ = name_ == L'x' ? x : y; }
};

Tree add(Tree a, Tree b);
Tree sub(Tree a, Tree b);
Tree mul(Tree a, Tree b);
Tree divide(Tree a, Tree b);

class Plus final : public Telement {
public:
    Plus(Tree a, Tree b) : Telement(std::move(a), std::move(b)) {}
    double rezult() const override { return checked(left->rezult() + right->rezult()); }
    Tree copy() const override { return std::make_unique<Plus>(left->copy(), right->copy()); }
    Tree differ() const override { return add(left->differ(), right->differ()); }
    std::wstring text() const override { return binary(L'+'); }
};

class Minus final : public Telement {
public:
    Minus(Tree a, Tree b) : Telement(std::move(a), std::move(b)) {}
    double rezult() const override { return checked(left->rezult() - right->rezult()); }
    Tree copy() const override { return std::make_unique<Minus>(left->copy(), right->copy()); }
    Tree differ() const override { return sub(left->differ(), right->differ()); }
    std::wstring text() const override { return binary(L'-'); }
};

class Mult final : public Telement {
public:
    Mult(Tree a, Tree b) : Telement(std::move(a), std::move(b)) {}
    double rezult() const override { return checked(left->rezult() * right->rezult()); }
    Tree copy() const override { return std::make_unique<Mult>(left->copy(), right->copy()); }
    Tree differ() const override {
        return add(mul(left->differ(), right->copy()), mul(left->copy(), right->differ()));
    }
    std::wstring text() const override { return binary(L'*'); }
};

class Div final : public Telement {
public:
    Div(Tree a, Tree b) : Telement(std::move(a), std::move(b)) {}
    double rezult() const override {
        const double den = right->rezult();
        if (den == 0) throw Error(L"Ділення на нуль.");
        return checked(left->rezult() / den);
    }
    Tree copy() const override { return std::make_unique<Div>(left->copy(), right->copy()); }
    Tree differ() const override {
        auto dr = right->differ();
        if (const auto* c = dynamic_cast<const Real*>(dr.get()); c && c->rezult() == 0)
            return divide(left->differ(), right->copy());
        return divide(sub(mul(left->differ(), right->copy()), mul(left->copy(), std::move(dr))),
                      mul(right->copy(), right->copy()));
    }
    std::wstring text() const override { return binary(L'/'); }
};

bool is_real(const Tree& node, double value) {
    const auto* p = dynamic_cast<const Real*>(node.get());
    return p && p->rezult() == value;
}

template<class Op>
Tree fold(Tree a, Tree b, wchar_t op) {
    const auto* v = dynamic_cast<const Real*>(a.get());
    const auto* w = dynamic_cast<const Real*>(b.get());
    if (v && w && (op != L'/' || w->rezult() != 0)) {
        const double x = v->rezult(), y = w->rezult();
        const double value = op == L'+' ? x + y : op == L'-' ? x - y
                           : op == L'*' ? x * y : x / y;
        if (std::isfinite(value)) return std::make_unique<Real>(value);
    }
    return std::make_unique<Op>(std::move(a), std::move(b));
}

Tree add(Tree a, Tree b) {
    if (is_real(a, 0)) return b;
    if (is_real(b, 0)) return a;
    return fold<Plus>(std::move(a), std::move(b), L'+');
}
Tree sub(Tree a, Tree b) {
    if (is_real(b, 0)) return a;
    return fold<Minus>(std::move(a), std::move(b), L'-');
}
Tree mul(Tree a, Tree b) {
    if (is_real(a, 0) || is_real(b, 0)) return std::make_unique<Real>(0);
    if (is_real(a, 1)) return b;
    if (is_real(b, 1)) return a;
    return fold<Mult>(std::move(a), std::move(b), L'*');
}
Tree divide(Tree a, Tree b) {
    if (is_real(b, 1)) return a;
    return fold<Div>(std::move(a), std::move(b), L'/');
}

class Sin final : public Telement {
public:
    explicit Sin(Tree a) : Telement(std::move(a)) {}
    double rezult() const override { return checked(std::sin(left->rezult())); }
    Tree copy() const override { return std::make_unique<Sin>(left->copy()); }
    Tree differ() const override;
    std::wstring text() const override { return L"sin(" + left->text() + L")"; }
};

class Cos final : public Telement {
public:
    explicit Cos(Tree a) : Telement(std::move(a)) {}
    double rezult() const override { return checked(std::cos(left->rezult())); }
    Tree copy() const override { return std::make_unique<Cos>(left->copy()); }
    Tree differ() const override {
        return sub(std::make_unique<Real>(0),
                   mul(std::make_unique<Sin>(left->copy()), left->differ()));
    }
    std::wstring text() const override { return L"cos(" + left->text() + L")"; }
};

Tree Sin::differ() const {
    return mul(std::make_unique<Cos>(left->copy()), left->differ());
}

class Parser {
    std::wstring_view input_;
    std::size_t pos_ = 0;
    bool allow_variables_;

    void skip() {
        while (pos_ < input_.size() && std::iswspace(input_[pos_])) ++pos_;
    }
    wchar_t peek() {
        skip();
        return pos_ < input_.size() ? input_[pos_] : L'\0';
    }
    bool take(wchar_t ch) {
        if (peek() != ch) return false;
        ++pos_;
        return true;
    }
    bool word(std::wstring_view name) {
        skip();
        if (input_.substr(pos_, name.size()) != name) return false;
        pos_ += name.size();
        return true;
    }
    [[noreturn]] void fail(const std::wstring& msg) const {
        throw Error(msg + L" Позиція: " + std::to_wstring(pos_ + 1) + L".");
    }

    Tree sum() {
        auto left = product();
        while (true) {
            if (take(L'+')) left = std::make_unique<Plus>(std::move(left), product());
            else if (take(L'-')) left = std::make_unique<Minus>(std::move(left), product());
            else return left;
        }
    }
    Tree product() {
        auto left = unary();
        while (true) {
            if (take(L'*')) left = std::make_unique<Mult>(std::move(left), unary());
            else if (take(L'/')) left = std::make_unique<Div>(std::move(left), unary());
            else return left;
        }
    }
    Tree unary() {
        if (take(L'+')) return unary();
        if (take(L'-')) return std::make_unique<Minus>(std::make_unique<Real>(0), unary());
        return atom();
    }
    Tree grouped() {
        if (!take(L'(')) fail(L"Очікується «(».");
        auto node = sum();
        if (!take(L')')) fail(L"Очікується «)».");
        return node;
    }
    static bool digit(wchar_t ch) { return ch >= L'0' && ch <= L'9'; }
    wchar_t raw() const { return pos_ < input_.size() ? input_[pos_] : L'\0'; }

    Tree number() {
        const auto begin = pos_;
        bool digits = false;
        while (digit(raw())) { ++pos_; digits = true; }
        if (raw() == L'.' || raw() == L',') {
            ++pos_;
            while (digit(raw())) { ++pos_; digits = true; }
        }
        if (!digits) fail(L"Очікується число, змінна або вираз у дужках.");
        if (raw() == L'e' || raw() == L'E') {
            ++pos_;
            if (raw() == L'+' || raw() == L'-') ++pos_;
            if (!digit(raw())) fail(L"Після показника степеня 10 потрібні цифри.");
            while (digit(raw())) ++pos_;
        }
        std::wstring token(input_.substr(begin, pos_ - begin));
        for (auto& ch : token) if (ch == L',') ch = L'.';
        std::wistringstream in(token);
        in.imbue(std::locale::classic());
        double value;
        if (!(in >> value) || !in.eof()) fail(L"Некоректне число.");
        return std::make_unique<Real>(checked(value));
    }
    Tree atom() {
        const auto ch = peek();
        if (ch == L'(') return grouped();
        if (word(L"sin")) return std::make_unique<Sin>(grouped());
        if (word(L"cos")) return std::make_unique<Cos>(grouped());
        const bool x = ch == L'x' || ch == L'X' || ch == L'х' || ch == L'Х';
        const bool y = ch == L'y' || ch == L'Y' || ch == L'у' || ch == L'У';
        if (x || y) {
            if (!allow_variables_) fail(L"У числовому полі змінні не дозволені.");
            ++pos_;
            return std::make_unique<Var>(x ? L'x' : L'y');
        }
        return number();
    }

public:
    Parser(std::wstring_view input, bool allow_variables)
        : input_(input), allow_variables_(allow_variables) {}
    Tree parse() {
        if (input_.size() > 512) throw Error(L"Вираз має містити не більше 512 символів.");
        auto root = sum();
        skip();
        if (pos_ != input_.size()) fail(L"Несподіваний символ.");
        return root;
    }
};

Tree form(std::wstring_view text, bool allow_variables = true) {
    return Parser(text, allow_variables).parse();
}

double evaluate(Telement& root, double x, double y = 0) {
    root.set_var(x, y);
    return root.rezult();
}

struct Calculated {
    double x = 0, y = 0, s = 0, v = 0, a = 0, third = 0, selected = 0;
    std::wstring formula;
};

Calculated calculate(std::wstring_view time, std::wstring_view path,
                     std::wstring_view parameter, unsigned order) {
    if (order < 1 || order > 20) throw Error(L"Введіть цілий порядок похідної від 1 до 20.");
    Calculated r;
    r.x = form(time, false)->rezult();
    if (parameter.find_first_not_of(L" \t\n\r") != std::wstring_view::npos)
        r.y = form(parameter, false)->rezult();
    const auto f = form(path);
    r.s = evaluate(*f, r.x, r.y); // Визначеність початкового виразу перевіряємо першою.

    const unsigned max_order = order > 3 ? order : 3;
    auto derivative = f->differ();
    for (unsigned n = 1; n <= max_order; ++n) {
        if (n == 1) r.v = evaluate(*derivative, r.x, r.y);
        if (n == 2) r.a = evaluate(*derivative, r.x, r.y);
        if (n == 3) r.third = evaluate(*derivative, r.x, r.y);
        if (n == order) {
            r.selected = evaluate(*derivative, r.x, r.y);
            r.formula = derivative->text();
        }
        if (n != max_order) derivative = derivative->differ();
    }
    return r;
}

std::wstring fixed(double value, int digits) {
    std::wostringstream out;
    out.imbue(std::locale::classic());
    out << std::fixed << std::setprecision(digits) << (value == 0 ? 0 : value);
    return out.str();
}

bool parse_order(std::wstring_view s, unsigned& value) {
    if (s.empty() || s.size() > 2) return false;
    value = 0;
    for (auto ch : s) {
        if (ch < L'0' || ch > L'9') return false;
        value = value * 10 + static_cast<unsigned>(ch - L'0');
    }
    return value >= 1 && value <= 20;
}
} // namespace oop11

#if defined(_WIN32) && !defined(OOP11_CONSOLE)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#if defined(_MSC_VER)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#endif

namespace {
using namespace oop11;
enum { TIME = 100, PATH, PARAM, ORDER, DIGITS, COMPUTE, CLOSE_APP };
struct Controls {
    HWND time{}, path{}, param{}, order{}, digits{};
    HWND x{}, s{}, v{}, a{}, third{}, selected{}, formula{}, status{};
    bool ready = false;
} ui;

HWND control(HWND parent, const wchar_t* kind, const wchar_t* value, DWORD style,
             int x, int y, int w, int h, int id = 0, DWORD extra = 0) {
    HWND item = CreateWindowExW(extra, kind, value, WS_CHILD | WS_VISIBLE | style,
        x, y, w, h, parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
        GetModuleHandleW(nullptr), nullptr);
    if (!item) throw std::runtime_error("Failed to create a control.");
    SendMessageW(item, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), TRUE);
    return item;
}

void label(HWND win, const wchar_t* t, int x, int y, int width = 190, int height = 21) {
    control(win, L"STATIC", t, SS_LEFT, x, y, width, height);
}

HWND edit(HWND win, const wchar_t* t, int x, int y, int w, int id = 0,
          bool ro = false, int h = 25) {
    auto style = DWORD(WS_TABSTOP) | (ro ? ES_READONLY : 0);
    style |= h > 30 ? ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL : ES_AUTOHSCROLL;
    auto item = control(win, L"EDIT", t, style, x, y, w, h, id, WS_EX_CLIENTEDGE);
    if (!ro) SendMessageW(item, EM_SETLIMITTEXT, id == ORDER ? 2 : 512, 0);
    return item;
}

std::wstring contents(HWND edit_box) {
    const int capacity = GetWindowTextLengthW(edit_box) + 1;
    std::wstring s(static_cast<std::size_t>(capacity), L'\0');
    s.resize(static_cast<std::size_t>(GetWindowTextW(edit_box, s.data(), capacity)));
    return s;
}

void show(HWND control, const std::wstring& text) {
    SetWindowTextW(control, text.c_str());
}

void clear_results() {
    for (HWND result : {ui.x, ui.s, ui.v, ui.a, ui.third, ui.selected, ui.formula})
        SetWindowTextW(result, L"");
}

void compute() {
    try {
        unsigned order = 0;
        if (!parse_order(contents(ui.order), order)) throw Error(L"Порядок похідної: ціле число від 1 до 20.");
        const int digits = static_cast<int>(SendMessageW(ui.digits, CB_GETCURSEL, 0, 0));
        const auto result = calculate(contents(ui.time), contents(ui.path), contents(ui.param), order);
        show(ui.x, fixed(result.x, digits));
        show(ui.s, fixed(result.s, digits));
        show(ui.v, fixed(result.v, digits));
        show(ui.a, fixed(result.a, digits));
        show(ui.third, fixed(result.third, digits));
        show(ui.selected, fixed(result.selected, digits));
        show(ui.formula, L"s^(" + std::to_wstring(order) + L")(x, y) = " + result.formula);
        show(ui.status, L"Обчислення завершено. Похідні взято за x; y вважаємо сталою.");
    } catch (const Error& e) {
        clear_results();
        show(ui.status, e.message);
    } catch (const std::exception&) {
        clear_results();
        show(ui.status, L"Не вдалося завершити обчислення.");
    }
}

void make_ui(HWND win) {
    label(win, L"Політ ракети — інтерпретатор математичних виразів", 18, 15, 575);
    label(win, L"Операції: +, -, *, /; дужки; sin і cos у радіанах. Десятковий роздільник: . або ,", 18, 43, 690);
    label(win, L"Вираз для часу x (с)", 18, 81);
    ui.time = edit(win, L"5,88+1,25/0,13698*3", 215, 78, 480, TIME);
    label(win, L"Закон руху s(x, y) (м)", 18, 115);
    ui.path = edit(win, L"8,3*x*x*x-12/x/x+0,13", 215, 112, 480, PATH);
    label(win, L"Параметр y (необов’язковий)", 18, 149);
    ui.param = edit(win, L"0", 215, 146, 145, PARAM);
    label(win, L"s диференціюється за x; значення y є сталим.", 375, 149, 320);

    label(win, L"Знаків після крапки", 18, 190, 180);
    ui.digits = control(win, L"COMBOBOX", L"", WS_TABSTOP | CBS_DROPDOWNLIST,
                        215, 185, 65, 180, DIGITS);
    for (int i = 0; i <= 8; ++i) {
        const auto digit = std::to_wstring(i);
        SendMessageW(ui.digits, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(digit.c_str()));
    }
    SendMessageW(ui.digits, CB_SETCURSEL, 4, 0);
    label(win, L"Порядок n (1–20)", 305, 190, 130);
    ui.order = edit(win, L"2", 444, 185, 45, ORDER);
    control(win, L"BUTTON", L"Обчислити", WS_TABSTOP | BS_DEFPUSHBUTTON, 507, 182, 105, 30, COMPUTE);
    control(win, L"BUTTON", L"Вихід", WS_TABSTOP | BS_PUSHBUTTON, 621, 182, 74, 30, CLOSE_APP);

    label(win, L"Результати обчислень", 18, 228, 420);
    label(win, L"Час x (с)", 18, 263);
    ui.x = edit(win, L"", 215, 260, 145, 0, true);
    label(win, L"Шлях s (м)", 385, 263, 155);
    ui.s = edit(win, L"", 550, 260, 145, 0, true);
    label(win, L"Швидкість s′ (м/с)", 18, 298);
    ui.v = edit(win, L"", 215, 295, 145, 0, true);
    label(win, L"Прискорення s″ (м/с²)", 385, 298, 165);
    ui.a = edit(win, L"", 550, 295, 145, 0, true);
    label(win, L"Третя похідна s‴", 18, 333);
    ui.third = edit(win, L"", 215, 330, 145, 0, true);
    label(win, L"Значення n-ї похідної", 385, 333, 165);
    ui.selected = edit(win, L"", 550, 330, 145, 0, true);

    label(win, L"Аналітичний вигляд n-ї похідної", 18, 370, 500);
    ui.formula = edit(win, L"", 18, 396, 677, 0, true, 85);
    ui.status = control(win, L"STATIC", L"Натисніть «Обчислити».", SS_LEFT, 18, 496, 677, 46);
    label(win, L"Сіроштанов Максим Володимирович | Ік-52 | Практичне заняття №11", 18, 551, 670);
    ui.ready = true;
}

LRESULT CALLBACK window_proc(HWND win, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            try { make_ui(win); } catch (const std::exception&) { return -1; }
            return 0;
        case WM_COMMAND:
            if (ui.ready) {
                const int id = LOWORD(wp), event = HIWORD(wp);
                if ((id == COMPUTE && event == BN_CLICKED) || (id == DIGITS && event == CBN_SELCHANGE)) {
                    compute();
                    return 0;
                }
                if (id == CLOSE_APP && event == BN_CLICKED) {
                    DestroyWindow(win);
                    return 0;
                }
                if ((id == TIME || id == PATH || id == PARAM || id == ORDER) && event == EN_CHANGE) {
                    clear_results();
                    show(ui.status, L"Дані змінено. Натисніть «Обчислити».");
                    return 0;
                }
            }
            break;
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(win, msg, wp, lp);
}
} // namespace

int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int show_cmd) {
    constexpr wchar_t klass[] = L"Oop11InterpreterWindow";
    WNDCLASSW wc{};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = klass;
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512)); // IDC_ARROW
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    if (!RegisterClassW(&wc)) return 1;
    constexpr DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT r{0, 0, 715, 585};
    AdjustWindowRect(&r, style, FALSE);
    HWND window = CreateWindowExW(0, klass, L"Політ ракети", style,
        CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top,
        nullptr, nullptr, instance, nullptr);
    if (!window) return 1;
    ShowWindow(window, show_cmd);
    MSG msg{};
    BOOL result = 0;
    while ((result = GetMessageW(&msg, nullptr, 0, 0)) > 0) {
        if (IsDialogMessageW(window, &msg)) continue;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return result == -1 ? 1 : static_cast<int>(msg.wParam);
}
#else
#include <iostream>
#include <clocale>

#ifndef OOP11_NO_MAIN
int main(int argc, char**) {
    using namespace oop11;
    std::setlocale(LC_ALL, "");
    std::wstring time = L"5,88+1,25/0,13698*3";
    std::wstring path = L"8,3*x*x*x-12/x/x+0,13";
    std::wstring parameter = L"0", order_text = L"2";
    if (argc > 1) {
        if (!std::getline(std::wcin, time) || !std::getline(std::wcin, path) ||
            !std::getline(std::wcin, parameter) || !std::getline(std::wcin, order_text)) {
            std::cerr << "Expected four lines: x, s(x,y), y, order.\n";
            return 1;
        }
    }
    try {
        unsigned order = 0;
        if (!parse_order(order_text, order)) throw Error(L"Порядок має бути від 1 до 20.");
        const auto r = calculate(time, path, parameter, order);
        std::wcout << L"x = " << fixed(r.x, 4) << L"\ns = " << fixed(r.s, 4)
                   << L"\nv = " << fixed(r.v, 4) << L"\na = " << fixed(r.a, 4)
                   << L"\ns''' = " << fixed(r.third, 4)
                   << L"\nD^" << order << L" = " << fixed(r.selected, 4)
                   << L"\nFormula: " << r.formula << L'\n';
        return 0;
    } catch (const Error& e) {
        std::wcerr << e.message << L'\n';
        return 2;
    }
}
#endif
#endif
