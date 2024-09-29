#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include "Dot.h"
#include "CalcedDotsAndSizes.h"
#include "IFunc.h"
#include "LinearFunc.h"
#include "SquareFunc.h"
#include "Derivative.h"
#include "Integral.h"

IFunc* functionToDraw = new SquareFunc(1, 1, 1);
Derivative* derivative = new Derivative(functionToDraw);
Integral* integral = new Integral(functionToDraw, 2);

const wchar_t CLASS_NAME[] = L"Graphics";

struct PenParams {
    int style, width;
    COLORREF color;
} axisPen{ PS_SOLID, 3, RGB(0, 0, 0) }, gridPen{ PS_SOLID, 1, RGB(0, 0, 0) }, graphPen{ PS_SOLID, 2, RGB(255, 0, 0) };


// Шаг сетки в пикселях
int STEP = 50;

// Разброс значений по оси X
int RANGE = 4;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS InitWindow(WNDCLASS& wc, HINSTANCE& hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
WPARAM StartMessageLoop();
void Render(HDC hdc);
void DrawGrid(HDC hdc, int width, int height);
void DrawNumbers(HDC hdc, int width, int height, double sizeCoeff);
void DrawXYAxis(HDC hdc, int width, int height);
void DrawGraph(HDC hdc, CalcedDotsAndSizes calcedDotsAndSizes, Dot center, double ySizeCoeff, double xSizeCoeff);
double CalcSizeCoeff(CalcedDotsAndSizes calcedDotsAndSizes, Dot center);
CalcedDotsAndSizes CalcDots(IFunc* func, double minX, double maxX, int width);
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
        L"Graphics",                    // Window text
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
    RECT rect;
    GetClientRect(WindowFromDC(hdc), &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    double xSizeCoeff = (double)width / 2 / (double)RANGE;

    Dot center = { width / 2, height / 2 };

    std::vector<double> sizeCoeffs;

    CalcedDotsAndSizes calcedDotsAndSizes = CalcDots(functionToDraw, -RANGE, RANGE, width);
    sizeCoeffs.push_back(CalcSizeCoeff(calcedDotsAndSizes, center));

    //CalcedDotsAndSizes derivativeDotsAndSizes = CalcDots(derivative, -RANGE, RANGE, width);
    CalcedDotsAndSizes derivativeDotsAndSizes = derivative->CalcDots(-RANGE, RANGE, 1000, xSizeCoeff);
    sizeCoeffs.push_back(CalcSizeCoeff(derivativeDotsAndSizes, center));

    CalcedDotsAndSizes integralDotsAndSizes = integral->CalcDots(-RANGE, RANGE, 1000, xSizeCoeff);
    sizeCoeffs.push_back(CalcSizeCoeff(integralDotsAndSizes, center));
    
    double sizeCoeff = sizeCoeffs[0];
    for (int i = 0; i < sizeCoeffs.size(); i++) {
        if (sizeCoeff > sizeCoeffs[i]) {
            sizeCoeff = sizeCoeffs[i];
        }
    }

    DrawGraph(hdc, calcedDotsAndSizes, center, sizeCoeff, xSizeCoeff);
    DrawGraph(hdc, derivativeDotsAndSizes, center, sizeCoeff, xSizeCoeff);
    DrawGraph(hdc, integralDotsAndSizes, center, sizeCoeff, xSizeCoeff);

    DrawXYAxis(hdc, width, height);
    DrawGrid(hdc, width, height);
    DrawNumbers(hdc, width, height, sizeCoeff);
}

double CalcSizeCoeff(CalcedDotsAndSizes calcedDotsAndSizes, Dot center) {
    YSizes ySizes = calcedDotsAndSizes.ySizes;

    double maxAbsY = ySizes.maxY;
    if (maxAbsY < ySizes.minY) maxAbsY = ySizes.minY;
    if (maxAbsY < -ySizes.minY) maxAbsY = -ySizes.minY;

    double sizeCoeff = (center.y / maxAbsY);
    return sizeCoeff;
}

void DrawGraph(HDC hdc, CalcedDotsAndSizes calcedDotsAndSizes, Dot center, double ySizeCoeff, double xSizeCoeff)
{
    SelectPen(hdc, graphPen);

    std::vector<Dot> dots = calcedDotsAndSizes.dots;
    for (int i = 0; i < dots.size(); i++) {
        Dot dot = dots[i];
        double y = (center.y - dot.y * ySizeCoeff);
        double x = (center.x + dot.x * xSizeCoeff);
        if (i == 0) {
            MoveToEx(hdc, x, y, NULL);
        }
        else {
            LineTo(hdc, x, y);
            //LineTo(hdc, x, y);
        }
    }
}

CalcedDotsAndSizes CalcDots(IFunc* func, double minX, double maxX, int width) {
    std::vector<Dot> dots;

    double xRange = maxX - minX;
    double step = xRange / width;

    double minY, maxY;

    for (double i = minX; i < maxX; i += step) {
        double x = i;
        double y = func->getValue(x);
        //double y = (*func)((double)i * RANGE / xRange);

        if (i == minX) {
            minY = y;
            maxY = y;
        }

        Dot dot;
        dot.x = x;
        dot.y = y;
        dots.push_back(dot);

        if (minY > y) minY = y;
        if (maxY < y) maxY = y;
    }

    YSizes ySizes;
    ySizes.minY = minY;
    ySizes.maxY = maxY;

    CalcedDotsAndSizes calcedDotsAndSizes;
    calcedDotsAndSizes.dots = dots;
    calcedDotsAndSizes.ySizes = ySizes;

    return calcedDotsAndSizes;
}

void DrawXYAxis(HDC hdc, int width, int height) 
{
    SelectPen(hdc, axisPen);
    // Ось X
    MoveToEx(hdc, 0, height / 2, NULL); 
    LineTo(hdc, width, height / 2); 

    // Стрелка на оси X
    MoveToEx(hdc, width, height / 2, NULL); 
    LineTo(hdc, width - 10, height / 2 - 5);
    MoveToEx(hdc, width, height / 2, NULL); 
    LineTo(hdc, width - 10, height / 2 + 5);

    // Ось Y
    MoveToEx(hdc, width / 2, 0, NULL);
    LineTo(hdc, width / 2, height); 

    // Стрелка на оси Y
    MoveToEx(hdc, width / 2, 0, NULL); 
    LineTo(hdc, width / 2 - 5, 10); 
    MoveToEx(hdc, width / 2, 0, NULL); 
    LineTo(hdc, width / 2 + 5, 10);
}

void DrawGrid(HDC hdc, int width, int height) {
    SelectPen(hdc, gridPen);

    // Вертикальные линии
    for (int x = width / 2 + STEP; x < width; x += STEP) {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, height);
    }
    for (int x = width / 2 - STEP; x > 0; x -= STEP) {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, height);
    }

    // Горизонтальные линии
    for (int y = height / 2 + STEP; y < height; y += STEP) {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, width, y);
    }
    for (int y = height / 2 - STEP; y > 0; y -= STEP) {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, width, y);
    }
}


void DrawNumbers(HDC hdc, int width, int height, double sizeCoeff) {
    int fontHeight = 20;
    
    HFONT hFont = CreateFont(fontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);

    // Числа на оси X
    for (int x = width / 2 + STEP; x < width; x += STEP) {
        std::wstringstream ss;
        double number = ((double)x - width / 2) * RANGE * 2 / width;
        double roundedNumber = std::round(number * 100) / 100;
        ss << roundedNumber;
        std::wstring str = ss.str();
        TextOut(hdc, x - 10, height / 2 - 20, str.c_str(), str.length());
    }
    for (int x = width / 2 - STEP; x > 0; x -= STEP) {
        std::wstringstream ss;
        double number = ((double)x - width / 2) * RANGE * 2 / width;
        double roundedNumber = std::round(number * 100) / 100;
        ss << roundedNumber;
        std::wstring str = ss.str();
        TextOut(hdc, x - 10, height / 2 - 20, str.c_str(), str.length());
    }

    // Числа на оси Y
    for (int y = height / 2 + STEP; y < height; y += STEP) {
        std::wstringstream ss;
        double number = -(y - height / 2) / sizeCoeff;
        double roundedNumber = std::round(number * 100) / 100;
        ss << roundedNumber;
        std::wstring str = ss.str();
        TextOut(hdc, width / 2 + 5, y - 10, str.c_str(), str.length());
    }
    for (int y = height / 2 - STEP; y > 0; y -= STEP) {
        std::wstringstream ss;
        double number = -(y - height / 2) / sizeCoeff;
        double roundedNumber = std::round(number * 100) / 100;
        ss << roundedNumber;
        std::wstring str = ss.str();
        TextOut(hdc, width / 2 + 5, y - 10, str.c_str(), str.length());
    }

    SelectObject(hdc, hOldFont);
}

void SelectPen(HDC hdc, PenParams penParams)
{
    HPEN coordinateAxesPen = CreatePen(penParams.style, penParams.width, penParams.color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, coordinateAxesPen); // Выбираем созданное перо
}




