#pragma once

#include "glm/glm.hpp"

inline float mouseSensitivity = 0.005f;

inline bool debug = false;

inline int windowWidth = 1280;
inline int windowHeight = 720;

inline int minFragmentSize = 1;

inline float a = 0.5f, b = 1.0f, c = 1.5f, scaleObj = 3.0f, m = 4.0f;
inline int startingAccuracy = 128;
inline int accuracy = startingAccuracy;

inline constexpr float floatDiff = 0.0001f;

inline glm::vec3 lightPos{ 0.0f, 0.0f, 10.0f };
inline glm::vec3 lightColor{ 1.0f, 1.0f, 0.0f };
inline float ambientStrength = 0.5f;
inline glm::vec3 ambientColor = lightColor * ambientStrength;

inline glm::vec3 diffuseColor = lightColor;
inline glm::vec3 specularColor = glm::vec3(1.f);

inline glm::vec3 viewPos = lightPos;

inline glm::vec4 objPos;

struct RendererValues
{
    float a, b, c, scaleObj, m;
    int accuracy;
    glm::vec4 objPos;
};

inline bool isUIclicked = false;