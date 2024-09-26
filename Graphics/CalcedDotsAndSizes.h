#pragma once

#include <vector>

#include "Dot.h"

struct YSizes {
    double minY, maxY;
};

struct CalcedDotsAndSizes {
    YSizes ySizes;
    std::vector<Dot> dots;
};