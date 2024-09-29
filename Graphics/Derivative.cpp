#include "Derivative.h"

Derivative::Derivative(IFunc* originalFunc) {
	this->originalFunc = originalFunc;
}

CalcedDotsAndSizes Derivative::CalcDots(double minX, double maxX, double n, double xSizeCoeff) {
	IFunc* f = this->originalFunc;

	CalcedDotsAndSizes calcedDots;

	double range = maxX - minX;
	double delta = range / n;
	double x = minX;
	for (double i = 0; i < n; i++) {
		double y = ((*f)(x) - (*f)(x - delta)) / delta;
		calcedDots.dots.push_back({ x, y });

		if (x == minX) {
			calcedDots.ySizes.maxY = y;
			calcedDots.ySizes.minY = y;
		}
		else {
			if (calcedDots.ySizes.minY > y) {
				calcedDots.ySizes.minY = y;
			}
			if (calcedDots.ySizes.maxY < y) {
				calcedDots.ySizes.maxY = y;
			}
		}

		x += delta;
	}

	return calcedDots;
}