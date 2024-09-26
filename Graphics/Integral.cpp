#include "Integral.h"

Integral::Integral(IFunc* originalFunc, double h) {
	this->originalFunc = originalFunc;
	this->h = h;
}

double Integral::getValue(double x) {
    double sum = 0;

    double x0 = x;
    double x1 = x + this->h;
    IFunc* f = this->originalFunc;
    
    sum = f->getValue(x0) + 4 * f->getValue(x0 + h / 2) + f->getValue(x1);

    return (h / 6) * sum;
}

double Integral::operator()(double x) {
    double sum = 0;

    double x0 = x;
    double x1 = x + this->h;
    IFunc* f = this->originalFunc;

    sum = f->getValue(x0) + 4 * f->getValue(x0 + h / 2) + f->getValue(x1);

    return (h / 6) * sum;
}