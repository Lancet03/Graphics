#pragma once
#include "IFunc.h"
#include "CalcedDotsAndSizes.h"

class Integral {
	IFunc* originalFunc;
	double h;
public:
	Integral(IFunc* func, double h);
	CalcedDotsAndSizes CalcDots(double a, double b, int steps, double xSizeCoeff);
};