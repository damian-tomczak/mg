#pragma once

#include <cassert>
#include <iostream>
#include <fstream>

#include "math.hpp"

inline float elipsoidZ(float x, float y, float a, float b, float c)
{
    assert(c != 0);

    float zSquared = -(a * x * x + b * y * y + -1 * 1 * 1) / c;

    return sqrt(zSquared);
}

inline float partialZPartialX(float x, float y, float a, float b, float c)
{
    assert(c != 0);

    float numerator = -a * x * sqrt(-((a * x * x + b * y * y - 1) / c));
    float denominator = -(a * x * x + b * y * y - 1);

    return numerator / denominator;
}

inline float partialZPartialY(float x, float y, float a, float b, float c)
{
    assert(c != 0);

    float numerator = -b * y * sqrt(-((a * x * x + b * y * y - 1) / c));
    float denominator = -(a * x * x + b * y * y - 1);

    return numerator / denominator;
}

inline float gradientMagnitude(float x, float y, float z, float a, float b, float c)
{
    float dx = partialZPartialX(x, y, a, b, c);
    float dy = partialZPartialY(x, y, a, b, c);

    return sqrt(dx * dx + dy * dy + z * z);
}

inline Vec3 normalVector(float x, float y, float z, float a, float b, float c)
{
    float dx = partialZPartialX(x, y, a, b, c);
    float dy = partialZPartialY(x, y, a, b, c);
    float mag = gradientMagnitude(x, y, z, a, b, c);


    //if (mag == 0)
    //{
    //    std::fstream f;
    //    f.open("out.txt", std::ios::out | std::ios::app);
    //    f << x << " " << y << "\n";
    //    f.close();

    //}

    float nx = dx / mag;
    float ny = dy / mag;

    float nz = 1 / mag;

    return {nx, ny, nz};
}