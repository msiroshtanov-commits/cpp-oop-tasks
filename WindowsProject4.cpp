#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

int activeLight = -1; // -1: вимкнено, 0: Червоний, 1: Жовтий, 2: Зелений

void OnLButtonDown(int mouseX, int mouseY) {
    if (mouseX >= 80 && mouseX <= 240) {
        if (mouseY >= 50 && mouseY <= 150) activeLight = 0;
        else if (mouseY >= 160 && mouseY <= 260) activeLight = 1;
        else if (mouseY >= 270 && mouseY <= 370) activeLight = 2;
    }
}

void DrawTrafficLight(Graphics& g) {
    // Корпус світлофора
    SolidBrush bodyBrush(Color(255, 50, 50, 50));
    g.FillRectangle(&bodyBrush, 80, 30, 160, 360);

    // Сигнали (активний горить яскраво, інші темні)
    SolidBrush red(activeLight == 0 ? Color(255, 255, 0, 0) : Color(255, 80, 0, 0));
    SolidBrush yellow(activeLight == 1 ? Color(255, 255, 255, 0) : Color(255, 80, 80, 0));
    SolidBrush green(activeLight == 2 ? Color(255, 0, 255, 0) : Color(255, 0, 80, 0));

    g.FillEllipse(&red, 110, 50, 100, 100);
    g.FillEllipse(&yellow, 110, 160, 100, 100);
    g.FillEllipse(&green, 110, 270, 100, 100);
}

// Головна віконна процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN: {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        OnLButtonDown(xPos, yPos);
        InvalidateRect(hWnd, NULL, TRUE); // Перемальовуємо вікно після кліку
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);
        DrawTrafficLight(graphics);
        EndPaint(hWnd, &ps);
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
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"GDIPlusWindowClass4";

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"GDIPlusWindowClass4", L"Лабораторна робота - Завдання 4 (Світлофор)",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 340, 470, NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}