#pragma once

class IFunc
{
public:
    virtual double getValue(double x) = 0;
    virtual double operator()(double x) = 0;
};

