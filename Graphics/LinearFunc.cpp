#include "LinearFunc.h"

LinearFunc::LinearFunc(double a = 1.0, double b = 0.0)
{
    this->a = a;
    this->b = b;
}

double LinearFunc::getValue(double x)
{
    return a * x + b;
}

double LinearFunc::operator()(double x)
{
    return a * x + b;
}