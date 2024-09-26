#pragma once
#include "IFunc.h"
#include "Dot.h"
#include "CalcedDotsAndSizes.h"

class Derivative : public IFunc {
	IFunc* originalFunc;
	double delta;
public:
	Derivative(IFunc* originalFunc, double delta);
	double getValue(double x) override;
	double operator()(double x) override;
};