#pragma once

#include <cassert>
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

inline float gradientMagnitude(float x, float y, float a, float b, float c)
{
    float dx = partialZPartialX(x, y, a, b, c);
    float dy = partialZPartialY(x, y, a, b, c);

    return sqrt(dx * dx + dy * dy);
}

inline Vec3 normalVector(float x, float y, float a, float b, float c)
{
    float dx = partialZPartialX(x, y, a, b, c);
    float dy = partialZPartialY(x, y, a, b, c);
    float mag = gradientMagnitude(x, y, a, b, c);

    //assert(mag != 0);

    float nx = dx / mag;
    float ny = dy / mag;

    float nz = 1 / mag;

    return {nx, ny, nz};
}