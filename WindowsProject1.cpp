#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

// Функція малювання діаграми (Завдання 1)
void OnPaint(HDC hdc, RECT clientRect) {
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    REAL rectW = width * 0.6f;
    REAL rectH = height * 0.5f;
    REAL rectX = (width - rectW) / 2.0f;
    REAL rectY = (height - rectH) / 2.0f;

    SolidBrush redBrush(Color(255, 255, 0, 0));
    SolidBrush greenBrush(Color(255, 0, 255, 0));
    SolidBrush blueBrush(Color(255, 0, 191, 255));
    SolidBrush textBrush(Color(255, 0, 0, 0));

    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 12, FontStyleBold, UnitPoint);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);

    // Сектор 1: 25% (90 градусів)
    graphics.FillPie(&redBrush, rectX, rectY, rectW, rectH, 0.0f, 90.0f);
    // Сектор 2: 65% (234 градуси)
    graphics.FillPie(&greenBrush, rectX, rectY, rectW, rectH, 90.0f, 234.0f);
    // Сектор 3: 10% (36 градусів)
    graphics.FillPie(&blueBrush, rectX, rectY, rectW, rectH, 324.0f, 36.0f);

    graphics.DrawString(L"25%", -1, &font, PointF(rectX + rectW * 0.75f, rectY + rectH * 0.75f), &format, &textBrush);
    graphics.DrawString(L"65%", -1, &font, PointF(rectX + rectW * 0.25f, rectY + rectH * 0.5f), &format, &textBrush);
    graphics.DrawString(L"10%", -1, &font, PointF(rectX + rectW * 0.85f, rectY + rectH * 0.35f), &format, &textBrush);
}

// Головна віконна процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        OnPaint(hdc, rect);
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
    // 1. Ініціалізація GDI+
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 2. Реєстрація класу вікна
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"GDIPlusWindowClass";
    RegisterClassExW(&wcex);

    // 3. Створення та відображення вікна
    HWND hWnd = CreateWindowW(L"GDIPlusWindowClass", L"Лабораторна робота - Завдання 1",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 600, 400, NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 4. Цикл повідомлень
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 5. Завершення роботи GDI+
    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}