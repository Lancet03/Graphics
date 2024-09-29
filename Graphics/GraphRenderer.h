#pragma once
#include <Windows.h>

#include "Dot.h"
#include "CalcedDotsAndSizes.h"
#include "PenParams.h"

class GraphRenderer {
	
private:	
	HDC hdc;
	int width;
	int height;

	int RANGE;
	int STEP;

	//PenParams axisPen{ PS_SOLID, 3, RGB(0, 0, 0) }, gridPen{ PS_SOLID, 1, RGB(0, 0, 0) }, graphPen{ PS_SOLID, 2, RGB(255, 0, 0) };

	void SelectPen(PenParams penParams);
public:
	GraphRenderer(HDC hdc, int width, int height, int range, int step);

	void DrawGraphPlane(double sizeCoeff);

	void DrawGrid(PenParams gridPen = { PS_SOLID, 1, RGB(0, 0, 0) });
	void DrawNumbers(double sizeCoeff);
	void DrawXYAxis(PenParams axisPen = { PS_SOLID, 3, RGB(0, 0, 0) });

	void DrawGraph(CalcedDotsAndSizes calcedDotsAndSizes, Dot center, double ySizeCoeff, double xSizeCoeff, PenParams graphPen = { PS_SOLID, 2, RGB(255, 0, 0) });
	
};

