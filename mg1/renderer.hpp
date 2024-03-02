#pragma once

#include "ui.hpp"
#include "math.hpp"
#include "equations.hpp"
#include "SDL.h"

#include <algorithm>
#include <thread>
#include <atomic>

class AdaptiveRenderer
{
public:
    inline static float adaptiveThreshold = 0.1f;
    static constexpr glm::vec4 backgroundColor{0.639f, 0.965f, 1.0f, 1.0f};

    AdaptiveRenderer() = default;

    void drawElipsoid(SDL_Texture* texture, int accuracy, const EllipsoidProperties& newProperties);

private:
    glm::vec4 computeColorAtCenter(int centerX, int centerY, const EllipsoidProperties& properties);
    void calculateFragment(int startX, int startY, int endX, int endY, Uint32* upixels, const EllipsoidProperties& properties);
    void processFragments(int startX, int startY, int fragmentWidth, int fragmentHeight, int accuracy, Uint32* upixels, const EllipsoidProperties& properties);
};

inline glm::vec4 AdaptiveRenderer::computeColorAtCenter(int centerX, int centerY, const EllipsoidProperties& properties)
{
    float normalizedX = (static_cast<float>(centerX) / windowWidth - 0.5f);
    float normalizedY = (0.5f - static_cast<float>(centerY) / windowHeight);

    glm::mat4 dPrim = calculateDPrim(
        properties.position.x, properties.position.y, properties.position.z,
        properties.scaleFactor, properties.scaleFactor, properties.scaleFactor,
        properties.rotation.x, properties.rotation.y, properties.rotation.z, properties.a, properties.b, properties.c);

    float z = elipsoidZ(normalizedX, normalizedY, dPrim);

    if (std::isnan(z))
    {
        return backgroundColor;
    }

    glm::vec3 point = {normalizedX, normalizedY, z};
    glm::vec3 normal = normalVector(point.x, point.y, point.z, dPrim);

    glm::vec3 ambient = lightColor * ambientStrength;

    glm::vec3 lightDir = normalize(lightPos - point);
    float diff = std::max(dot(normal, lightDir), 0.f);
    glm::vec3 diffuse = diff * lightColor;

    glm::vec3 viewDir = normalize(viewPos - point);
    glm::vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(std::max(dot(viewDir, reflectDir), 0.f), properties.m);
    glm::vec3 specular = spec * glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 finalColor = ambient + diffuse + specular;
    finalColor.x = std::clamp(finalColor.x, 0.f, 1.f);
    finalColor.y = std::clamp(finalColor.y, 0.f, 1.f);
    finalColor.z = std::clamp(finalColor.z, 0.f, 1.f);

    return {finalColor.x, finalColor.y, finalColor.z, 1.f};
}

inline void AdaptiveRenderer::calculateFragment(int startX, int startY, int endX, int endY, Uint32* upixels, const EllipsoidProperties& properties)
{
    int centerX = (startX + endX) / 2;
    int centerY = (startY + endY) / 2;

    glm::vec4 color = computeColorAtCenter(centerX, centerY, properties);

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

inline void AdaptiveRenderer::processFragments(int startX, int startY, int fragmentWidth, int fragmentHeight, int accuracy, Uint32* upixels, const EllipsoidProperties& properties)
{
    if ((fragmentWidth <= accuracy) || (fragmentHeight <= accuracy))
    {
        calculateFragment(startX, startY, startX + fragmentWidth, startY + fragmentHeight, upixels, properties);
        return;
    }

    int halfWidth = fragmentWidth / 2;
    int halfHeight = fragmentHeight / 2;

    processFragments(startX, startY, halfWidth, halfHeight, accuracy, upixels, properties);
    processFragments(startX + halfWidth, startY, fragmentWidth - halfWidth, halfHeight, accuracy, upixels, properties);
    processFragments(startX, startY + halfHeight, halfWidth, fragmentHeight - halfHeight, accuracy, upixels, properties);
    processFragments(startX + halfWidth, startY + halfHeight, fragmentWidth - halfWidth, fragmentHeight - halfHeight, accuracy, upixels, properties);
}

inline void AdaptiveRenderer::drawElipsoid(SDL_Texture* texture, int accuracy, const EllipsoidProperties& properties)
{
    Uint32* pixels;
    int pitch;

    if (SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch) < 0)
    {
        return;
    }

    int totalBytes = pitch * windowHeight;
    memset(pixels, 0, totalBytes);

    processFragments(0, 0, windowWidth, windowHeight, accuracy, pixels, properties);

    SDL_UnlockTexture(texture);
}