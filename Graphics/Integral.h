#pragma once
#include "IFunc.h"
#include "CalcedDotsAndSizes.h"

class Integral: public IFunc{
	IFunc* originalFunc;
	double h;
public:
	Integral(IFunc* func, double h);
	double getValue(double x) override;
	double operator()(double x) override;

	CalcedDotsAndSizes CalcDots(double a, double b, int steps, double xSizeCoeff);
};