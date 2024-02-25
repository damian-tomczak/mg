#pragma once

#include "math.hpp"

inline bool debug = false;

inline int windowWidth = 1280;
inline int windowHeight = 720;

inline int minFragmentSize = 1;

inline float a = 0.5f, b = 1.0f, c = 1.5f, scale = 3.0f, m = 4.0f;
inline int startingAccuracy = 128;
inline int accuracy = startingAccuracy;

inline constexpr float floatDiff = 0.0001f;

inline Vec3 lightPos{ 0.0f, 0.0f, 10.0f };
inline Vec3 lightColor{ 1.0f, 1.0f, 0.0f };
inline float ambientStrength = 0.5f;
inline Vec3 ambientColor = lightColor * ambientStrength;

inline Vec3 diffuseColor = lightColor;
inline Vec3 specularColor = Vec3(1.f);

inline Vec3 viewPos = lightPos;

struct PreviousValues
{
    float a, b, c, scale, m;
    int accuracy;
};