#pragma once
#include "IFunc.h"

class SquareFunc : public IFunc
{
private:
    double a, b, c;
public:
    SquareFunc(double a, double b, double c);
    double getValue(double x) override;
    double operator()(double x);
};

