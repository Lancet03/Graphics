#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <sstream>

const wchar_t CLASS_NAME[] = L"Graphics";

struct PenParams {
    int style, width;
    COLORREF color;
} axisPen{ PS_SOLID, 3, RGB(0, 0, 0) }, gridPen{ PS_SOLID, 1, RGB(0, 0, 0) }, graphPen{ PS_SOLID, 2, RGB(255, 0, 0) };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS InitWindow(WNDCLASS& wc, HINSTANCE& hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
WPARAM StartMessageLoop();
void Render(HDC hdc);
void DrawGrid(HDC hdc, int width, int height);
void DrawNumbers(HDC hdc, int width, int height);
void DrawXYAxis(HDC hdc, int width, int height);
void SelectPen(HDC hdc, PenParams penParams);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASS wc = { };

    InitWindow(wc, hInstance);

    HWND hwnd = CreateMainWindow(hInstance);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    return StartMessageLoop();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        Render(hdc);

        EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_SIZE:
    {
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WNDCLASS InitWindow(WNDCLASS& wc, HINSTANCE& hInstance) 
{
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClass(&wc);

    return wc;
}

HWND CreateMainWindow(HINSTANCE hInstance) 
{
    HWND hWnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Graphics",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    return hWnd;
}

WPARAM StartMessageLoop() 
{
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void Render(HDC hdc) 
{
    // Размеры окна
    RECT rect;
    GetClientRect(WindowFromDC(hdc), &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Рисуем оси координат
    DrawXYAxis(hdc, width, height);
    
    // Рисуем координатную сетку
    DrawGrid(hdc, width, height);

    // Рисуем числа на пересечениях сетки
    DrawNumbers(hdc, width, height);
}

void DrawXYAxis(HDC hdc, int width, int height) 
{
    SelectPen(hdc, axisPen);

    MoveToEx(hdc, 0, height / 2, NULL); // Перемещаем перо в начало координат
    LineTo(hdc, width, height / 2); // Рисуем ось X
    MoveToEx(hdc, width / 2, 0, NULL); // Перемещаем перо в начало координат
    LineTo(hdc, width / 2, height); // Рисуем ось Y

}

void DrawGrid(HDC hdc, int width, int height) {
    SelectPen(hdc, gridPen);

    // Параметры сетки (шаг)
    int step = 50;

    // Рисуем вертикальные линии
    for (int x = width / 2 + step; x < width; x += step) {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, height);
    }
    for (int x = width / 2 - step; x > 0; x -= step) {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, height);
    }

    // Рисуем горизонтальные линии
    for (int y = height / 2 + step; y < height; y += step) {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, width, y);
    }
    for (int y = height / 2 - step; y > 0; y -= step) {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, width, y);
    }
}

// Функция для рисования чисел на пересечениях сетки
void DrawNumbers(HDC hdc, int width, int height) {
    int fontSize = 20;

    // Параметры сетки (шаг)
    int step = 50;

    // Шрифт для чисел
    HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // Рисуем числа на оси X
    for (int x = width / 2 + step; x < width; x += step) {
        std::wstringstream ss;
        ss << (x - width / 2);
        std::wstring str = ss.str();
        TextOut(hdc, x - 10, height / 2 - 20, str.c_str(), str.length());
    }
    for (int x = width / 2 - step; x > 0; x -= step) {
        std::wstringstream ss;
        ss << (x - width / 2);
        std::wstring str = ss.str();
        TextOut(hdc, x - 10, height / 2 - 20, str.c_str(), str.length());
    }

    // Рисуем числа на оси Y
    for (int y = height / 2 + step; y < height; y += step) {
        std::wstringstream ss;
        ss << -(y - height / 2);
        std::wstring str = ss.str();
        TextOut(hdc, width / 2 + 5, y - 10, str.c_str(), str.length());
    }
    for (int y = height / 2 - step; y > 0; y -= step) {
        std::wstringstream ss;
        ss << -(y - height / 2);
        std::wstring str = ss.str();
        TextOut(hdc, width / 2 + 5, y - 10, str.c_str(), str.length());
    }

    // Восстанавливаем предыдущий шрифт
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void SelectPen(HDC hdc, PenParams penParams)
{
    HPEN coordinateAxesPen = CreatePen(penParams.style, penParams.width, penParams.color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, coordinateAxesPen); // Выбираем созданное перо
}