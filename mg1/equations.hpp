#pragma once

#include <cassert>
#include <iostream>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

inline glm::mat4 calculateDPrim(
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

    return MInverseTranspose * diagonalMatrix * MInverse;
}

inline float elipsoidZ(float x, float y, const glm::mat4& dPrim)
{
    float A = dPrim[2][2];
    float B = dPrim[2][3] + dPrim[3][2] + dPrim[0][2] * x + dPrim[2][0] * x + dPrim[1][2] * y + dPrim[2][1] * y;
    float C = dPrim[3][3] + dPrim[0][0] * x * x + dPrim[0][3] * x + dPrim[3][0] * x + dPrim[1][1] * y * y + dPrim[1][3] * y + dPrim[3][1] * y + dPrim[0][1] * x * y + dPrim[1][0] * x * y;

    float discriminant = B * B - 4 * A * C;

    float z1 = (-B + std::sqrt(discriminant)) / (2 * A);
    float z2 = (-B - std::sqrt(discriminant)) / (2 * A);

    return z1 > z2 ? z1 : z2;
}

inline glm::vec3 normalVector(float x, float y, float z, const glm::mat4& dPrim)
{
    double df_dx = 2 * dPrim[0][0] * x + dPrim[1][0] * y + dPrim[1][0] * y + dPrim[2][0] * z + dPrim[2][0] * z + dPrim[0][3] + dPrim[3][0];
    double df_dy = dPrim[1][0] * x + dPrim[1][0] * x + 2 * dPrim[1][1] * y + dPrim[1][2] * z + dPrim[1][3] + dPrim[2][1] * z + dPrim[3][1];
    double df_dz = dPrim[1][2] * y + dPrim[2][0] * x + dPrim[2][0] * x + dPrim[2][1] * y + 2 * dPrim[2][2] * z + dPrim[2][3] + dPrim[3][2];

    const glm::vec3 normal{df_dx, df_dy, df_dz};

    return glm::normalize(normal);
}