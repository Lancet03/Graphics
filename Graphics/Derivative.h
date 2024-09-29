#pragma once
#include "IFunc.h"
#include "Dot.h"
#include "CalcedDotsAndSizes.h"

class Derivative {
	IFunc* originalFunc;
public:
	Derivative(IFunc* originalFunc);
	CalcedDotsAndSizes CalcDots(double a, double b, double delta, double xSizeCoeff);
	
	/*double getValue(double x);
	double operator()(double x);*/
};