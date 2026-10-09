#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

// Завдання 2: Функція побудови діаграми з довільними параметрами
void DrawCustomPieChart(Graphics& g, RectF bounds, const float* percents, const Color* colors, int count) {
    float startAngle = 0.0f;
    for (int i = 0; i < count; ++i) {
        float sweepAngle = (percents[i] / 100.0f) * 360.0f;
        SolidBrush brush(colors[i]);
        g.FillPie(&brush, bounds.X, bounds.Y, bounds.Width, bounds.Height, startAngle, sweepAngle);
        startAngle += sweepAngle;
    }
}

// Викликаємо функцію DrawCustomPieChart при перемальовуванні вікна
void OnPaint(HDC hdc, RECT clientRect) {
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    // Прямокутник для розміщення діаграми
    RectF bounds((REAL)(width * 0.2), (REAL)(height * 0.2), (REAL)(width * 0.6), (REAL)(height * 0.6));

    // Масиви з довільними відсотками та кольорами для тестування
    float percents[] = { 40.0f, 30.0f, 20.0f, 10.0f };
    Color colors[] = {
        Color(255, 255, 99, 71),   // Червоний / Томатний
        Color(255, 60, 179, 113),  // Зелений
        Color(255, 30, 144, 255),  // Блакитний
        Color(255, 255, 215, 0)    // Жовтий
    };

    // Виклик функції Завдання 2
    DrawCustomPieChart(graphics, bounds, percents, colors, 4);
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
    wcex.lpszClassName = L"GDIPlusWindowClass2";
    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"GDIPlusWindowClass2", L"Лабораторна робота - Завдання 2",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 600, 400, NULL, NULL, hInstance, NULL);

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