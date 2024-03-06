#pragma once

#include "glm/glm.hpp"

inline constexpr float floatDiff = 0.0001f;

inline bool debug = false;

inline constexpr int windowWidth = 1280;
inline constexpr int windowHeight = 720;

inline glm::vec3 lightPos{ 0.0f, 0.0f, 10.0f };
inline glm::vec3 lightColor{ 1.0f, 1.0f, 0.0f };
inline float ambientStrength = 0.5f;
inline glm::vec3 ambientColor = lightColor * ambientStrength;

inline glm::vec3 diffuseColor = lightColor;
inline glm::vec3 specularColor = glm::vec3(1.f);

inline glm::vec3 viewPos = lightPos;

inline int startingAccuracy = 9;
inline int accuracyCounter = startingAccuracy;

struct EllipsoidProperties
{
    float a = 0.5f, b = 1.0f, c = 1.5f;
    float scaleFactor = 0.2f;
    float m = 4.0f;
    glm::vec4 position = glm::vec4(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);

    auto operator<=>(const EllipsoidProperties& other) const = default;
};