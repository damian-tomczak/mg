#pragma once

#include <cassert>
#include <iostream>
#include <fstream>

#include "math.hpp"

inline float elipsoidZ(float x, float y, float tx, float ty, float tz, float a, float b, float c)
{
    float discriminant = sqrt(c * (-a * tx * tx + 2 * a * tx * x - a * x * x - b * ty * ty + 2 * b * ty * y - b * y * y + 1));

    float z1 = (c * tz - discriminant) / c;
    float z2 = (c * tz + discriminant) / c;

    return z2;
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

    return sqrt(dx * dx + dy * dy);
}

inline glm::vec3 normalVector(float x, float y, float z, float a, float b, float c)
{
    //float dx = partialZPartialX(x, y, a, b, c);
    //float dy = partialZPartialY(x, y, a, b, c);
    //float mag = gradientMagnitude(x, y, z, a, b, c);

    //float nx = dx / mag;
    //float ny = dy / mag;

    //float nz = 1 / mag;

    //return {nx, ny, nz};

    glm::vec3 grad = { 2 * a * x, 2 * b * y, 2 * c * z };

    float magnitude = sqrt(grad.x * grad.x + grad.y * grad.y + grad.z * grad.z);

    glm::vec3 normal = { grad.x / magnitude, grad.y / magnitude, grad.z / magnitude };

    return normal;
}