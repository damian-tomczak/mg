#pragma once

#include <cassert>
#include <iostream>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

inline float elipsoidZ(float x, float y,
    float tx, float ty, float tz,
    float sx, float sy, float sz,
    float rx, float ry, float rz,
    float a, float b, float c)
{
    glm::mat4 diagonalMatrix = glm::mat4(0.f);
    diagonalMatrix[0][0] = a;
    diagonalMatrix[1][1] = b;
    diagonalMatrix[2][2] = c;
    diagonalMatrix[3][3] = -1;

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));

    float alpha = glm::radians(rx);
    float beta = glm::radians(ry);
    float gamma = glm::radians(rz);

    glm::mat4 rotateX(1.0f);
    rotateX[1][1] = cos(alpha);
    rotateX[1][2] = -sin(alpha);
    rotateX[2][1] = sin(alpha);
    rotateX[2][2] = cos(alpha);

    glm::mat4 rotateY(1.0f);
    rotateY[0][0] = cos(beta);
    rotateY[0][2] = sin(beta);
    rotateY[2][0] = -sin(beta);
    rotateY[2][2] = cos(beta);

    glm::mat4 rotateZ(1.0f);
    rotateZ[0][0] = cos(gamma);
    rotateZ[0][1] = -sin(gamma);
    rotateZ[1][0] = sin(gamma);
    rotateZ[1][1] = cos(gamma);

    glm::mat4 rotationMatrix = rotateZ * rotateY * rotateX;

    glm::mat4 MMatrix = scaleMatrix * rotationMatrix * rotationMatrix * translationMatrix;

    glm::mat4 MInverse = glm::inverse(MMatrix);
    glm::mat4 MInverseTranspose = glm::transpose(MInverse);

    glm::mat4 DPrim = MInverseTranspose * diagonalMatrix * MInverse;

    float A = DPrim[2][2];
    float B = DPrim[2][3] + DPrim[3][2] + DPrim[0][2] * x + DPrim[2][0] * x + DPrim[1][2] * y + DPrim[2][1] * y;
    float C = DPrim[3][3] + DPrim[0][0] * x * x + DPrim[0][3] * x + DPrim[3][0] * x + DPrim[1][1] * y * y + DPrim[1][3] * y + DPrim[3][1] * y + DPrim[0][1] * x * y + DPrim[1][0] * x * y;

    float discriminant = B * B - 4 * A * C;

    float z1 = (-B + std::sqrt(discriminant)) / (2 * A);
    float z2 = (-B - std::sqrt(discriminant)) / (2 * A);

    return z1 > z2 ? z1 : z2;
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