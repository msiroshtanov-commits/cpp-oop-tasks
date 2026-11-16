#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define NOMINMAX

#include <windows.h>
#include <cmath>
#include <cwchar>
#include <iomanip>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// ======================================================
// КЛАСИ ВЕРШИН ДЕРЕВА
// ======================================================

struct Context {
    double x = 0;
    double y = 0;
};

class Telement {
protected:
    std::unique_ptr<Telement> left;
    std::unique_ptr<Telement> right;

    Telement(
        std::unique_ptr<Telement> l = nullptr,
        std::unique_ptr<Telement> r = nullptr
    ) : left(std::move(l)), right(std::move(r)) {
    }

public:
    virtual ~Telement() = default;

    virtual double rezult(const Context& context) const = 0;
};

using Node = std::unique_ptr<Telement>;

double checked(double value) {
    if (!std::isfinite(value)) {
        throw std::runtime_error("Numeric overflow.");
    }

    return value;
}

class Number final : public Telement {
    double value;

public:
    explicit Number(double v) : value(v) {}

    double rezult(const Context&) const override {
        return value;
    }
};

class Variable final : public Telement {
    char name;

public:
    explicit Variable(char n) : name(n) {}

    double rezult(const Context& context) const override {
        return name == 'x' ? context.x : context.y;
    }
};

class Plus final : public Telement {
public:
    Plus(Node l, Node r)
        : Telement(std::move(l), std::move(r)) {
    }

    double rezult(const Context& context) const override {
        return checked(
            left->rezult(context) + right->rezult(context)
        );
    }
};

class Mult final : public Telement {
public:
    Mult(Node l, Node r)
        : Telement(std::move(l), std::move(r)) {
    }

    double rezult(const Context& context) const override {
        return checked(
            left->rezult(context) * right->rezult(context)
        );
    }
};

// ======================================================
// ПОБУДОВА ДЕРЕВА З РЯДКА
// ======================================================

class Parser {
    const std::wstring& text;
    size_t pos = 0;
    bool allowVariables;

    void spaces() {
        while (pos < text.size() &&
            (text[pos] == L' ' || text[pos] == L'\t')) {
            ++pos;
        }
    }

    bool take(wchar_t symbol) {
        spaces();

        if (pos < text.size() && text[pos] == symbol) {
            ++pos;
            return true;
        }

        return false;
    }

    static bool digit(wchar_t symbol) {
        return symbol >= L'0' && symbol <= L'9';
    }

    [[noreturn]] void error() const {
        throw std::runtime_error(
            "Invalid expression at position " +
            std::to_string(pos + 1)
        );
    }

    Node operand() {
        spaces();

        if (pos >= text.size()) {
            error();
        }

        if (text[pos] == L'x' || text[pos] == L'y') {
            if (!allowVariables) {
                throw std::runtime_error(
                    "Variables are forbidden in definitions of x and y."
                );
            }

            char name = static_cast<char>(text[pos++]);
            return std::make_unique<Variable>(name);
        }

        const size_t start = pos;
        bool hasDigit = false;

        while (pos < text.size() && digit(text[pos])) {
            hasDigit = true;
            ++pos;
        }

        if (pos < text.size() &&
            (text[pos] == L'.' || text[pos] == L',')) {
            ++pos;

            while (pos < text.size() && digit(text[pos])) {
                hasDigit = true;
                ++pos;
            }
        }

        if (!hasDigit) {
            error();
        }

        std::wstring token = text.substr(start, pos - start);

        for (wchar_t& symbol : token) {
            if (symbol == L',') {
                symbol = L'.';
            }
        }

        std::wistringstream input(token);
        input.imbue(std::locale::classic());

        double value = 0;

        if (!(input >> value) || !std::isfinite(value)) {
            throw std::runtime_error("Invalid or too large number.");
        }

        return std::make_unique<Number>(value);
    }

    Node product() {
        Node root = operand();

        while (take(L'*')) {
            Node next = operand();

            root = std::make_unique<Mult>(
                std::move(root),
                std::move(next)
            );
        }

        return root;
    }

    Node sum() {
        Node root = product();

        while (take(L'+')) {
            Node next = product();

            root = std::make_unique<Plus>(
                std::move(root),
                std::move(next)
            );
        }

        return root;
    }

public:
    Parser(const std::wstring& expression, bool variables)
        : text(expression), allowVariables(variables) {
    }

    Node parse() {
        if (text.size() > 1024) {
            throw std::runtime_error("Expression is too long.");
        }

        Node root = sum();

        spaces();

        if (pos != text.size()) {
            error();
        }

        return root;
    }
};

Node form(const std::wstring& text, bool variables = true) {
    return Parser(text, variables).parse();
}

// ======================================================
// ВІКОННИЙ ІНТЕРФЕЙС
// ======================================================

enum {
    ID_X = 101,
    ID_Y,
    ID_FORMULA,
    ID_PRECISION,
    ID_COMPUTE,
    ID_EXAMPLE,
    ID_EXIT
};

HWND editX = nullptr;
HWND editY = nullptr;
HWND editFormula = nullptr;
HWND comboPrecision = nullptr;

HWND resultX = nullptr;
HWND resultY = nullptr;
HWND resultFunction = nullptr;
HWND resultExample = nullptr;
HWND statusText = nullptr;

HFONT interfaceFont = nullptr;

int screenDpi = 96;

int scaled(int value) {
    return MulDiv(value, screenDpi, 96);
}

std::wstring getText(HWND control) {
    int length = GetWindowTextLengthW(control);

    std::vector<wchar_t> buffer(
        static_cast<size_t>(length) + 1
    );

    GetWindowTextW(control, buffer.data(), length + 1);

    return buffer.data();
}

std::wstring formatNumber(double value, int precision) {
    std::wostringstream output;
    output.imbue(std::locale::classic());

    output << std::fixed
        << std::setprecision(precision)
        << value;

    return output.str();
}

std::wstring widen(const char* text) {
    // Повідомлення винятків у цій версії містять лише ASCII.
    std::wstring result;

    while (*text) {
        result += static_cast<unsigned char>(*text++);
    }

    return result;
}

HWND addControl(
    HWND parent,
    const wchar_t* className,
    const wchar_t* text,
    DWORD style,
    int x,
    int y,
    int width,
    int height,
    int id = 0
) {
    bool isEdit = std::wcscmp(className, L"EDIT") == 0;

    HWND control = CreateWindowExW(
        isEdit ? WS_EX_CLIENTEDGE : 0,
        className,
        text,
        WS_CHILD | WS_VISIBLE | style,
        scaled(x),
        scaled(y),
        scaled(width),
        scaled(height),
        parent,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
        GetModuleHandleW(nullptr),
        nullptr
    );

    SendMessageW(
        control,
        WM_SETFONT,
        reinterpret_cast<WPARAM>(interfaceFont),
        TRUE
    );

    if (isEdit && !(style & ES_READONLY)) {
        SendMessageW(control, EM_SETLIMITTEXT, 1024, 0);
    }

    return control;
}

void addLabel(
    HWND parent,
    const wchar_t* text,
    int x,
    int y,
    int width,
    int height = 25
) {
    addControl(
        parent, L"STATIC", text, 0,
        x, y, width, height
    );
}

void clearResults() {
    SetWindowTextW(resultX, L"");
    SetWindowTextW(resultY, L"");
    SetWindowTextW(resultFunction, L"");
}

void calculate(HWND window, bool showError) {
    try {
        int precision = static_cast<int>(
            SendMessageW(comboPrecision, CB_GETCURSEL, 0, 0)
            );

        if (precision < 0 || precision > 8) {
            throw std::runtime_error("Select precision from 0 to 8.");
        }

        // Спочатку обчислюємо числові вирази для змінних.
        auto treeX = form(getText(editX), false);
        auto treeY = form(getText(editY), false);

        Context empty;
        Context context;

        context.x = treeX->rezult(empty);
        context.y = treeY->rezult(empty);

        // Потім будуємо та обчислюємо дерево функції.
        auto functionTree = form(getText(editFormula));
        double value = functionTree->rezult(context);

        SetWindowTextW(
            resultX,
            formatNumber(context.x, precision).c_str()
        );

        SetWindowTextW(
            resultY,
            formatNumber(context.y, precision).c_str()
        );

        SetWindowTextW(
            resultFunction,
            formatNumber(value, precision).c_str()
        );

        SetWindowTextW(
            statusText,
            L"Обчислення завершено."
        );
    }
    catch (const std::exception& error) {
        clearResults();

        std::wstring message =
            L"Помилка у виразі. " + widen(error.what());

        SetWindowTextW(statusText, message.c_str());

        if (showError) {
            MessageBoxW(
                window,
                message.c_str(),
                L"Перевірте введення",
                MB_OK | MB_ICONWARNING
            );
        }
    }
}

void calculateExample() {
    Context context;

    auto a = std::make_unique<Number>(3);
    double valueA = a->rezult(context);

    auto m = std::make_unique<Mult>(
        std::move(a),
        std::make_unique<Number>(4)
    );

    double valueM = m->rezult(context);

    auto p = std::make_unique<Plus>(
        std::move(m),
        std::make_unique<Number>(2)
    );

    double valueP = p->rezult(context);

    std::wstring result =
        L"p = " + formatNumber(valueP, 2) +
        L"; m = " + formatNumber(valueM, 2) +
        L"; a = " + formatNumber(valueA, 2);

    SetWindowTextW(resultExample, result.c_str());
}

LRESULT CALLBACK WindowProc(
    HWND window,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (message) {
    case WM_CREATE:
        interfaceFont = CreateFontW(
            -scaled(15), 0, 0, 0,
            FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH,
            L"Segoe UI"
        );

        addLabel(
            window,
            L"Інтерпретатор математичних виразів",
            24, 20, 760, 30
        );

        addLabel(
            window,
            L"Дозволено числа, x, y та операції + і *.",
            24, 58, 760
        );

        addControl(
            window, L"BUTTON", L"Вхідні вирази",
            BS_GROUPBOX, 24, 96, 790, 200
        );

        addLabel(window, L"Вираз для x", 44, 132, 150);

        editX = addControl(
            window, L"EDIT", L"2+3",
            ES_AUTOHSCROLL | WS_TABSTOP,
            210, 128, 575, 29, ID_X
        );

        addLabel(window, L"Вираз для y", 44, 180, 150);

        editY = addControl(
            window, L"EDIT", L"3*2",
            ES_AUTOHSCROLL | WS_TABSTOP,
            210, 176, 575, 29, ID_Y
        );

        addLabel(window, L"f(x) або f(x, y)", 44, 228, 160);

        editFormula = addControl(
            window, L"EDIT", L"x*x+2*x+3",
            ES_AUTOHSCROLL | WS_TABSTOP,
            210, 224, 575, 29, ID_FORMULA
        );

        addLabel(
            window,
            L"Для x та y вводьте лише числа, + і *. Роздільник: . або ,",
            44, 266, 730
        );

        addControl(
            window, L"BUTTON", L"Результати обчислення",
            BS_GROUPBOX, 24, 310, 790, 130
        );

        addLabel(window, L"x =", 44, 350, 36);

        resultX = addControl(
            window, L"EDIT", L"",
            ES_READONLY | ES_AUTOHSCROLL,
            83, 346, 215, 29
        );

        addLabel(window, L"y =", 325, 350, 36);

        resultY = addControl(
            window, L"EDIT", L"",
            ES_READONLY | ES_AUTOHSCROLL,
            365, 346, 215, 29
        );

        addLabel(window, L"f =", 44, 398, 36);

        resultFunction = addControl(
            window, L"EDIT", L"",
            ES_READONLY | ES_AUTOHSCROLL,
            83, 394, 697, 29
        );

        addLabel(
            window, L"Знаків після крапки",
            24, 462, 180
        );

        comboPrecision = addControl(
            window, L"COMBOBOX", L"",
            CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,
            210, 457, 80, 250, ID_PRECISION
        );

        for (int i = 0; i <= 8; ++i) {
            std::wstring value = std::to_wstring(i);

            SendMessageW(
                comboPrecision,
                CB_ADDSTRING,
                0,
                reinterpret_cast<LPARAM>(value.c_str())
            );
        }

        SendMessageW(
            comboPrecision, CB_SETCURSEL, 2, 0
        );

        addControl(
            window, L"BUTTON", L"Обчислити",
            BS_DEFPUSHBUTTON | WS_TABSTOP,
            320, 455, 170, 34, ID_COMPUTE
        );

        addControl(
            window, L"BUTTON", L"Вихід",
            BS_PUSHBUTTON | WS_TABSTOP,
            640, 455, 174, 34, ID_EXIT
        );

        addControl(
            window, L"BUTTON", L"Дерево 3*4+2",
            BS_PUSHBUTTON | WS_TABSTOP,
            24, 510, 190, 34, ID_EXAMPLE
        );

        resultExample = addControl(
            window, L"STATIC",
            L"Натисніть кнопку для обчислення p, m та a.",
            0, 234, 515, 580, 48
        );

        statusText = addControl(
            window, L"STATIC",
            L"Натисніть «Обчислити».",
            0, 24, 578, 790, 55
        );

        addLabel(
            window,
            L"Сіроштанов Максим Володимирович | Ік-52",
            24, 645, 790
        );

        SetFocus(editX);
        return 0;

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        int event = HIWORD(wParam);

        if (id == ID_EXIT) {
            DestroyWindow(window);
            return 0;
        }

        if (id == ID_COMPUTE && event == BN_CLICKED) {
            calculate(window, true);
            return 0;
        }

        if (id == ID_PRECISION && event == CBN_SELCHANGE) {
            calculate(window, false);
            return 0;
        }

        if (id == ID_EXAMPLE && event == BN_CLICKED) {
            calculateExample();
            return 0;
        }

        bool expressionChanged =
            id == ID_X ||
            id == ID_Y ||
            id == ID_FORMULA;

        if (expressionChanged &&
            event == EN_CHANGE &&
            statusText != nullptr) {
            clearResults();

            SetWindowTextW(
                statusText,
                L"Вираз змінено. Натисніть «Обчислити»."
            );
        }

        break;
    }

    case WM_CLOSE:
        DestroyWindow(window);
        return 0;

    case WM_DESTROY:
        DeleteObject(interfaceFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(
        window, message, wParam, lParam
    );
}

// ======================================================
// ТОЧКА ВХОДУ
// ======================================================

int WINAPI wWinMain(
    HINSTANCE instance,
    HINSTANCE,
    PWSTR,
    int show
) {
    SetProcessDPIAware();

    HDC screen = GetDC(nullptr);
    screenDpi = GetDeviceCaps(screen, LOGPIXELSX);
    ReleaseDC(nullptr, screen);

    WNDCLASSW windowClass{};

    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = L"InterpreterWindow";

    windowClass.hCursor = LoadCursorW(
        nullptr, IDC_ARROW
    );

    windowClass.hbrBackground =
        reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);

    if (!RegisterClassW(&windowClass)) {
        return 1;
    }

    DWORD style = WS_CAPTION;

    RECT rectangle{
        0, 0, scaled(840), scaled(700)
    };

    AdjustWindowRect(&rectangle, style, FALSE);

    HWND window = CreateWindowExW(
        WS_EX_CONTROLPARENT,
        windowClass.lpszClassName,
        L"Обчислення виразу",
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rectangle.right - rectangle.left,
        rectangle.bottom - rectangle.top,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if (!window) {
        return 1;
    }

    ShowWindow(window, show);
    UpdateWindow(window);

    MSG message{};

    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        if (!IsDialogMessageW(window, &message)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    return static_cast<int>(message.wParam);
}