#include "Derivative.h"

Derivative::Derivative(IFunc* originalFunc, double delta) {
	this->originalFunc = originalFunc;
	this->delta = delta;
}

double Derivative::getValue(double x) {
	IFunc* y = this->originalFunc;

	return ((*y)(x + this->delta) - (*y)(x)) / this->delta;
}

double Derivative::operator()(double x) {
	IFunc* y = this->originalFunc;

	return ((*y)(x + this->delta) - (*y)(x)) / this->delta;
}