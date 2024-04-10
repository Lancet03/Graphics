#include<iostream>
#include<memory>
#include "IFunc.h"
#include "LinearFunc.h"
#include "SquareFunc.h"
int main()
{
    // std::shared_ptr<IFunc> lf = std::make_shared<LinearFunc>(2.0, -5.5);
    // std::shared_ptr<IFunc> sf = std::make_shared<SquareFunc>(4.0, -2.0, 3.0);
    LinearFunc lf(2.0, -5.5);
    SquareFunc sf(4.0, -2.0, 3.0);

    for (double x = -5.0; x < 5.0; x += 0.5)
        std::cout << "X = " << x << ", Y = " << lf(sf(x)) << std::endl;

    return 0;
}