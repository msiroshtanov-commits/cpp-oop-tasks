#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

// Завдання 5: Виведення тексту в 3 стовпчики зі зміною шрифту та кольорів
void Draw3Columns(Graphics& g, int width, int height) {
    FontFamily ff(L"Arial");

    for (int i = 0; i < 5; ++i) {
        // Випадковий колір для кожного рядка
        SolidBrush brush(Color(255, rand() % 200, rand() % 200, rand() % 200));

        // Лівий стовпчик: розмір шрифту зростає згори донизу (10, 14, 18, 22, 26)
        Gdiplus::Font fontL(&ff, (REAL)(10 + i * 4), FontStyleRegular, UnitPoint);
        g.DrawString(L"Привіт", -1, &fontL, PointF(20.0f, (REAL)(1 + i * 60 + 20)), &brush);

        // Середній стовпчик: розмір шрифту зростає знизу догори (26, 22, 18, 14, 10)
        Gdiplus::Font fontM(&ff, (REAL)(26 - i * 4), FontStyleRegular, UnitPoint);
        g.DrawString(L"Привіт", -1, &fontM, PointF((REAL)(width / 2 - 40), (REAL)(i * 60 + 20)), &brush);

        // Правий стовпчик: розмір шрифту зростає згори донизу (10, 14, 18, 22, 26)
        Gdiplus::Font fontR(&ff, (REAL)(10 + i * 4), FontStyleRegular, UnitPoint);
        g.DrawString(L"Привіт", -1, &fontR, PointF((REAL)(width - 150), (REAL)(1 + i * 60 + 20)), &brush);
    }
}

// Головна віконна процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);

        RECT rect;
        GetClientRect(hWnd, &rect);
        Draw3Columns(graphics, rect.right - rect.left, rect.bottom - rect.top);

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
    wcex.lpszClassName = L"GDIPlusWindowClass5";

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"GDIPlusWindowClass5", L"Лабораторна робота - Завдання 5 (3 стовпчики)",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 700, 400, NULL, NULL, hInstance, NULL);

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