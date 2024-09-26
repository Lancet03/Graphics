#pragma once
#include "IFunc.h"

class Integral: public IFunc{
	IFunc* originalFunc;
	double h;
public:
	Integral(IFunc* func, double h);
	double getValue(double x) override;
	double operator()(double x) override;
};