#include "SquareFunc.h"

SquareFunc::SquareFunc(double a = 1.0, double b = 0.0, double c = 0.0)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

double SquareFunc::getValue(double x)
{
    return a * x * x + b * x + c;
}

double SquareFunc::operator()(double x)
{
    return a * x * x + b * x + c;
}