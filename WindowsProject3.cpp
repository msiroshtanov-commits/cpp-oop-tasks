#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

// Завдання 3: Побудова спектра кольорів
void DrawSpectrum(HDC hdc, int width, int height) {
    Graphics g(hdc);
    for (int x = 0; x < width; ++x) {
        float t = (float)x / (float)width;
        int r = 0, g_col = 0, b = 0;

        if (t <= 0.25f) {
            r = 255;
            g_col = (int)(255 * (t / 0.25f));
            b = 0;
        }
        else if (t <= 0.5f) {
            r = (int)(255 * (1.0f - (t - 0.25f) / 0.25f));
            g_col = 255;
            b = 0;
        }
        else if (t <= 0.75f) {
            r = 0;
            g_col = 255;
            b = (int)(255 * ((t - 0.5f) / 0.25f));
        }
        else {
            r = 0;
            g_col = (int)(255 * (1.0f - (t - 0.75f) / 0.25f));
            b = 255;
        }

        Pen pen(Color(255, r, g_col, b));
        g.DrawLine(&pen, x, 0, x, height);
    }
}

// Головна віконна процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        DrawSpectrum(hdc, rect.right - rect.left, rect.bottom - rect.top);
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
    wcex.lpszClassName = L"GDIPlusWindowClass3";

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"GDIPlusWindowClass3", L"Лабораторна робота - Завдання 3 (Спектр)",
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