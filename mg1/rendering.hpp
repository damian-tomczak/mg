#pragma once

#include "ui.hpp"
#include "math.hpp"
#include "equations.hpp"
#include "SDL.h"

#include <algorithm>

inline glm::vec4 computeColorAtCenter(int centerX, int centerY)
{
    float normalizedX = (static_cast<float>(centerX) / windowWidth - 0.5f) * scaleObj;
    float normalizedY = (0.5f - static_cast<float>(centerY) / windowHeight) * scaleObj;

    float z = elipsoidZ(normalizedX, normalizedY, objPos.x, objPos.y, objPos.z, a, b, c);
    if (std::isnan(z))
    {
        return glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    }

    glm::vec3 point = {normalizedX, normalizedY, z};
    glm::vec3 normal = normalVector(point.x, point.y, point.z, a, b, c);

    glm::vec3 ambient = lightColor * ambientStrength;

    glm::vec3 lightDir = normalize(lightPos - point);
    float diff = std::max(dot(normal, lightDir), 0.f);
    glm::vec3 diffuse = diff * lightColor;

    glm::vec3 viewDir = normalize(viewPos - point);
    glm::vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(std::max(dot(viewDir, reflectDir), 0.f), m);
    glm::vec3 specular = spec * glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 finalColor = ambient + diffuse + specular;
    finalColor.x = std::clamp(finalColor.x, 0.f, 1.f);
    finalColor.y = std::clamp(finalColor.y, 0.f, 1.f);
    finalColor.z = std::clamp(finalColor.z, 0.f, 1.f);

    return glm::vec4{finalColor.x, finalColor.y, finalColor.z, 1.f};
}

inline void calculateFragment(int startX, int startY, int endX, int endY, Uint32* upixels)
{
    int centerX = (startX + endX) / 2;
    int centerY = (startY + endY) / 2;

    glm::vec4 color = computeColorAtCenter(centerX, centerY);

    for (int y = startY; y < endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            upixels[y * windowWidth + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888),
                static_cast<Uint8>(color.x * 255), static_cast<Uint8>(color.y * 255),
                static_cast<Uint8>(color.z * 255), static_cast<Uint8>(color.w * 255));
        }
    }
}

inline void processFragments(int startX, int startY, int fragmentWidth, int fragmentHeight, int accuracy, Uint32* upixels)
{
    if ((fragmentWidth <= accuracy) || (fragmentHeight <= accuracy))
    {
        calculateFragment(startX, startY, startX + fragmentWidth, startY + fragmentHeight, upixels);
        return;
    }

    int halfWidth = fragmentWidth / 2;
    int halfHeight = fragmentHeight / 2;

    processFragments(startX, startY, halfWidth, halfHeight, accuracy, upixels);
    processFragments(startX + halfWidth, startY, fragmentWidth - halfWidth, halfHeight, accuracy, upixels);
    processFragments(startX, startY + halfHeight, halfWidth, fragmentHeight - halfHeight, accuracy, upixels);
    processFragments(startX + halfWidth, startY + halfHeight, fragmentWidth - halfWidth, fragmentHeight - halfHeight, accuracy, upixels);
}