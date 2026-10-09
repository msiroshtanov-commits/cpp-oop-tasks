#include <windows.h>

#define IDC_BUTTON1 101
#define IDC_LISTBOX1 102

HWND hListBox;
HWND hButton;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // Створення ListBox на всю робочу область (Завдання 9, 10)
        hListBox = CreateWindowEx(0, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
            0, 50, 0, 0, hWnd, (HMENU)IDC_LISTBOX1, GetModuleHandle(NULL), NULL);

        // Наповнення елементами списку
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Елемент списку 1");
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Елемент списку 2");

        // Кнопка для завантаження зображення (Завдання 7)
        hButton = CreateWindowEx(0, L"BUTTON", L"Завантажити зображення",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 10, 180, 30, hWnd, (HMENU)IDC_BUTTON1, GetModuleHandle(NULL), NULL);
        break;
    }
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        // Розтягування ListBox під розмір вікна (alClient)
        MoveWindow(hListBox, 0, 50, width, height - 50, TRUE);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_BUTTON1) {
            MessageBox(hWnd, L"Зображення завантажено (Image1->Picture->LoadFromFile)", L"Інфо", MB_OK);
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"VCL_Emulation_Class";

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"VCL_Emulation_Class", L"C++ Builder VCL Завдання 6-10",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 500, 400, NULL, NULL, hInstance, NULL);

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