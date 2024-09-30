#include "GraphRenderer.h"

#include <iostream>
#include <sstream>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

GraphRenderer::GraphRenderer(HDC hdc, int width, int height, int range, int step) {
    this->hdc = hdc;
    this->width = width;
    this->height = height;
    this->RANGE = range;
    this->STEP = step;
}

void GraphRenderer::DrawGraph(CalcedDotsAndSizes calcedDotsAndSizes, Dot center, double ySizeCoeff, double xSizeCoeff, GdiplusPenParams graphPen)
{
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Pen      pen(graphPen.color, graphPen.width);
    //Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0), 2);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

    //SelectPen(graphPen);

    std::vector<Dot> dots = calcedDotsAndSizes.dots;
    for (int i = 0; i < dots.size() - 1; i++) {
        Dot dot = dots[i];
        int y = (center.y - dot.y * ySizeCoeff);
        int x = (center.x + dot.x * xSizeCoeff);

        Dot dot1 = dots[i + 1];
        int y1 = (center.y - dot1.y * ySizeCoeff);
        int x1 = (center.x + dot1.x * xSizeCoeff);
        //if (i == 0) {
        //    MoveToEx(hdc, x, y, NULL);
        //}
        //else {
        //    //LineTo(hdc, x, y);
        //    graphics.DrawLine(&pen, x, y, 200, 100);
        //}
        graphics.DrawLine(&pen, x, y, x1, y1);
    }
}

void GraphRenderer::DrawGraphPlane(double sizeCoeff) {
    this->DrawGrid();
    this->DrawNumbers(sizeCoeff);
    this->DrawXYAxis();
}

void GraphRenderer::DrawNumbers(double sizeCoeff) {
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

void GraphRenderer::DrawXYAxis(PenParams axisPen)
{
    SelectPen(axisPen);
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

void GraphRenderer::DrawGrid(PenParams gridPen) {
    SelectPen(gridPen);

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

void GraphRenderer::DrawGraphInfo(HDC hdc, std::vector<GraphInfoParam> graphInfoParams) {
    const int OFFSET_X = 20;
    const int OFFSET_Y = 10;

    int infoBoxLeft = OFFSET_X;
    int infoBoxTop = OFFSET_Y;
    int infoBoxRight = OFFSET_X + 250;
    int infoBoxBottom = OFFSET_Y + 30 * graphInfoParams.size() + 10;

    // Обводка области
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Чёрная обводка
    SelectObject(hdc, hPen);
    Rectangle(hdc, infoBoxLeft, infoBoxTop, infoBoxRight, infoBoxBottom);

    for (int i = 0; i < graphInfoParams.size(); i++) {
        GraphInfoParam param = graphInfoParams[i];
        this->SelectPen(param.penParams);
        //SelectObject(hdc, );
        int yPos = OFFSET_Y + 20 + 30 * i;
        MoveToEx(hdc, OFFSET_X + 10, yPos, NULL);
        LineTo(hdc, OFFSET_X + 110, yPos);
        TextOut(hdc, OFFSET_X + 120, yPos - 10, param.graphName.c_str(), param.graphName.size());
    }
}

void GraphRenderer::SelectPen(PenParams penParams)
{
    HPEN coordinateAxesPen = CreatePen(penParams.style, penParams.width, penParams.color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, coordinateAxesPen); // Выбираем созданное перо
}

