#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <gdiplus.h>
#include <objidl.h>
#pragma comment (lib,"Gdiplus.lib")

#include "Dot.h"
#include "CalcedDotsAndSizes.h"
#include "IFunc.h"
#include "LinearFunc.h"
#include "SquareFunc.h"
#include "Derivative.h"
#include "Integral.h"
#include "PenParams.h"
#include "GraphRenderer.h"

const wchar_t CLASS_NAME[] = L"Graphics";

// Шаг сетки в пикселях
int STEP = 50;
// Разброс значений по оси X
int RANGE = 4;

IFunc* functionToDraw = new SquareFunc(1, 1, 1);
Derivative* derivative = new Derivative(functionToDraw);
Integral* integral = new Integral(functionToDraw, 2);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS InitWindow(WNDCLASS& wc, HINSTANCE& hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
WPARAM StartMessageLoop();
void Render(HDC hdc);
double CalcSizeCoeff(CalcedDotsAndSizes calcedDotsAndSizes, Dot center);
CalcedDotsAndSizes CalcDots(IFunc* func, double minX, double maxX, int width);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASS wc = { };
    MSG                 msg;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    InitWindow(wc, hInstance);

    HWND hwnd = CreateMainWindow(hInstance);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return msg.wParam;
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
    GdiplusPenParams originalFuncPen = { 2, Gdiplus::Color(255, 0, 0) };

    CalcedDotsAndSizes derivativeDotsAndSizes = derivative->CalcDots(-RANGE, RANGE, 1000, xSizeCoeff);
    sizeCoeffs.push_back(CalcSizeCoeff(derivativeDotsAndSizes, center));
    GdiplusPenParams  derivativePen = { 2, Gdiplus::Color(0, 255, 0) };

    CalcedDotsAndSizes integralDotsAndSizes = integral->CalcDots(-RANGE, RANGE, 1000, xSizeCoeff);
    sizeCoeffs.push_back(CalcSizeCoeff(integralDotsAndSizes, center));
    GdiplusPenParams  integralPen = { 2, Gdiplus::Color(0, 0, 255) };
    
    // Выбор наименьшего размерного коэффициента для отображения всех графиков полностью в одном окне
    double sizeCoeff = sizeCoeffs[0];
    for (int i = 0; i < sizeCoeffs.size(); i++) {
        if (sizeCoeff > sizeCoeffs[i]) {
            sizeCoeff = sizeCoeffs[i];
        }
    }

    GraphRenderer graphRenderer(hdc, width, height, RANGE, STEP);

    graphRenderer.DrawGraph(calcedDotsAndSizes, center, sizeCoeff, xSizeCoeff, originalFuncPen);
    graphRenderer.DrawGraph(derivativeDotsAndSizes, center, sizeCoeff, xSizeCoeff, derivativePen);
    graphRenderer.DrawGraph(integralDotsAndSizes, center, sizeCoeff, xSizeCoeff, integralPen);

    graphRenderer.DrawGraphPlane(sizeCoeff);

    std::vector<GraphInfoParam> graphInfoParams = { 
        { originalFuncPen, L"Функция" }, 
        { derivativePen, L"Производная"}, 
        { integralPen, L"Интеграл"} 
    };

    graphRenderer.DrawGraphInfo(hdc, graphInfoParams);
}

double CalcSizeCoeff(CalcedDotsAndSizes calcedDotsAndSizes, Dot center) {
    YSizes ySizes = calcedDotsAndSizes.ySizes;

    double maxAbsY = ySizes.maxY;
    if (maxAbsY < ySizes.minY) maxAbsY = ySizes.minY;
    if (maxAbsY < -ySizes.minY) maxAbsY = -ySizes.minY;

    double sizeCoeff = (center.y / maxAbsY);
    return sizeCoeff;
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
