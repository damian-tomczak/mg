#pragma once

#include "ui.hpp"
#include "math.hpp"
#include "equations.h"
#include "SDL.h"

#include <algorithm>
#include <thread>
#include <atomic>

inline SDL_PixelFormat* format;

class AdaptiveRenderer
{
public:
    inline static float adaptiveThreshold = 10.f;
    static constexpr glm::vec4 backgroundColor{ 0.639f, 0.965f, 1.0f, 1.0f };

    AdaptiveRenderer() = default;

    void drawElipsoid(SDL_Texture* texture, int accuracy, const EllipsoidProperties& newProperties);

private:
    glm::vec4 computeColor(int centerX, int centerY, const EllipsoidProperties& properties);
};
