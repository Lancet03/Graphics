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

    sum += f->getValue(x0) + 4 * f->getValue(x0 + h / 2) + f->getValue(x1);

    return (h / 6) * sum;
}

CalcedDotsAndSizes Integral::CalcDots(double a, double b, int n, double xSizeCoeff) {
    const double width = (b - a) / n;
    IFunc* f = this->originalFunc;

    CalcedDotsAndSizes calcedDots;

    double simpson_integral = 0;
    for (int step = 0; step <= n; step++) {
        const double x1 = a + step * width;
        const double x2 = a + (step + 1) * width;

        simpson_integral += (x2 - x1) / 6.0 * (f->getValue(x1) + 4.0 * f->getValue(0.5 * (x1 + x2)) + f->getValue(x2));

        calcedDots.dots.push_back({ x1, simpson_integral });

        if (step == 0) {
            calcedDots.ySizes.maxY = simpson_integral;
            calcedDots.ySizes.minY = simpson_integral;
        }
        else {
            if (calcedDots.ySizes.minY > simpson_integral) {
                calcedDots.ySizes.minY = simpson_integral;
            }
            if (calcedDots.ySizes.maxY < simpson_integral) {
                calcedDots.ySizes.maxY = simpson_integral;
            }
        }
    }
    
    /*double h = (b - a) / n;
    double sum = 0;

    double x0 = a;
    double x1 = a + h;

    IFunc* f = this->originalFunc;

    CalcedDotsAndSizes calcedDots;


    for (double i = a; i <= b; i += h) {
        sum += f->getValue(x0 * xSizeCoeff) + 4 * f->getValue((x0 + h / 2) * xSizeCoeff) + f->getValue(x1 * xSizeCoeff);

        x0 += h;
        x1 += h;

        double y = (h / 6) * sum;
        calcedDots.dots.push_back({ x0, y });

        if (i == a) {
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
        }*/
    //}

    return calcedDots;
}