#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "globals.hpp"
#include "renderer.h"

#include <iostream>
#include <string>

enum class InteractionType
{
    MOVE,
    ROTATE
};

class Menu
{
public:
    bool isUIclicked = false;

    Menu(float& mouseSensitivty) : mMouseSensitivity{mouseSensitivty}
    {}

    void renderMenu(EllipsoidProperties& properties, float deltaTime);
    float getMenuWidth();
    bool getMenuEnabled() { return mIsMenuEnabled; }
    InteractionType getInteractionType() { return mInteractionType; }

private:
    bool mIsMenuEnabled = false;
    InteractionType mInteractionType{};
    float& mMouseSensitivity;
};

inline void Menu::renderMenu(EllipsoidProperties& properties, float deltaTime)
{
    mIsMenuEnabled = false;

    const ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    const float menuWidth = getMenuWidth();

    ImGui::SetNextWindowPos(ImVec2(windowSize.x - menuWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(menuWidth, windowSize.y));

    if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        mIsMenuEnabled = true;

        ImGui::Text("Ellipsoid:");

        isUIclicked = false;

        ImGui::Spacing();

        static const char* comboItems[] = {"MOVE", "ROTATE"};

        ImGui::Combo("##InteractionType", reinterpret_cast<int*>(&mInteractionType), comboItems, IM_ARRAYSIZE(comboItems));

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("a", &properties.a, 0.0f, 10.0f);                          isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("b", &properties.b, 0.0f, 10.0f);                          isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("c", &properties.c, 0.1f, 10.0f);                          isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("scale", &properties.scaleFactor, 0.1f, 1.0f);             isUIclicked |= ImGui::IsItemActive();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("m", &properties.m, 0.1f, 10.0f);                          isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderInt("accuracy", &properties.accuracy, blockSize, blockSize * blockSize); isUIclicked |= ImGui::IsItemActive();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Mouse sensitivity: ");
        ImGui::SliderFloat("##mouseSensivity", &mMouseSensitivity, 0.1f, 100.0f);

        ImGui::Text(("DeltaTime: " + std::to_string(deltaTime)).c_str());

        if (ImGui::Button("Reset ellipsoid"))
        {
            properties.position = glm::vec4(0.f);
            properties.rotation = glm::vec4(0.f);

            isUIclicked = true;
        }

    }

    ImGui::End();
}

inline float Menu::getMenuWidth()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float menuWidth = windowSize.x * 0.2f;

    return menuWidth;
}