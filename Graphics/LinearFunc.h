#pragma once

#include "IFunc.h"

class LinearFunc : public IFunc
{
private:
    double a, b;
public:
    LinearFunc(double a, double b);
    double getValue(double x) override;
    double operator()(double x);
};
