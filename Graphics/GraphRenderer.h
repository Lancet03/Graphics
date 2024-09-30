#pragma once
#include <Windows.h>
#include <sstream>

#include "Dot.h"
#include "CalcedDotsAndSizes.h"
#include "PenParams.h"

struct GraphInfoParam {
	PenParams penParams;
	std::wstring graphName;
};

class GraphRenderer {
	
private:	
	HDC hdc;
	int width;
	int height;

	int RANGE;
	int STEP;

	void SelectPen(PenParams penParams);
public:
	GraphRenderer(HDC hdc, int width, int height, int range, int step);

	void DrawGraphPlane(double sizeCoeff);

	void DrawGrid(PenParams gridPen = { PS_SOLID, 1, RGB(0, 0, 0) });
	void DrawNumbers(double sizeCoeff);
	void DrawXYAxis(PenParams axisPen = { PS_SOLID, 3, RGB(0, 0, 0) });

	void DrawGraph(CalcedDotsAndSizes calcedDotsAndSizes, Dot center, double ySizeCoeff, double xSizeCoeff, GdiplusPenParams graphPen = { 2, Gdiplus::Color(255, 0, 0, 255) });
	void DrawGraphInfo(HDC hdc, std::vector<GraphInfoParam> graphInfoParams);
};

