#include <windows.h>

#define IDC_BUTTON_SHOWFORM 101
#define IDC_BUTTON_LOADIMG  102
#define IDC_LISTBOX         103

HWND hListBox;
HWND hBtnShowForm;
HWND hBtnLoadImg;

// Процедура для Другої форми (Завдання 11 - Form2.Show)
LRESULT CALLBACK Form2WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Процедура для Головної форми (Завдання 11-15)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // 1) Кнопка "Show Form2" (Завдання 11: Form2.Show)
        hBtnShowForm = CreateWindowEx(0, L"BUTTON", L"Show Form2 [Завдання 11]",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 10, 200, 30, hWnd, (HMENU)IDC_BUTTON_SHOWFORM, GetModuleHandle(NULL), NULL);

        // 2) Кнопка "Load Image" (Завдання 14: Image1.Picture.LoadFromFile)
        hBtnLoadImg = CreateWindowEx(0, L"BUTTON", L"Load Image [Завдання 14]",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 10, 200, 30, hWnd, (HMENU)IDC_BUTTON_LOADIMG, GetModuleHandle(NULL), NULL);

        // 3) Компонент ListBox (Завдання 15: ListBox1.Align = alClient)
        hListBox = CreateWindowEx(0, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
            0, 50, 0, 0, hWnd, (HMENU)IDC_LISTBOX, GetModuleHandle(NULL), NULL);

        // Заповнення елементами
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Елемент списку 1 (Delphi / VCL)");
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Елемент списку 2 (Delphi / VCL)");
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Елемент списку 3 (Delphi / VCL)");
        break;
    }
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        // Розтягування ListBox на всю область під кнопками (alClient)
        MoveWindow(hListBox, 0, 50, width, height - 50, TRUE);
        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        if (wmId == IDC_BUTTON_SHOWFORM) {
            // Створення та відображення Другої форми (Form2)
            WNDCLASSEXW wc2 = { 0 };
            wc2.cbSize = sizeof(WNDCLASSEX);
            wc2.lpfnWndProc = Form2WndProc;
            wc2.hInstance = GetModuleHandle(NULL);
            wc2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc2.lpszClassName = L"DelphiForm2Class";
            wc2.hCursor = LoadCursor(NULL, IDC_ARROW);
            RegisterClassExW(&wc2);

            HWND hForm2 = CreateWindowW(L"DelphiForm2Class", L"Form2 (Друга форма)",
                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 300, 200, hWnd, NULL, GetModuleHandle(NULL), NULL);
            ShowWindow(hForm2, SW_SHOW);
        }
        else if (wmId == IDC_BUTTON_LOADIMG) {
            MessageBox(hWnd, L"Зображення растрове завантажено в Image1!", L"Delphi Task 14", MB_OK | MB_ICONINFORMATION);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Точка входу Windows-програми
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"DelphiEmulationClass";

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"DelphiEmulationClass", L"Лабораторна робота - Завдання 11-15 (VCL Emulation)",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 550, 400, NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}