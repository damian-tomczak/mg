#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "globals.hpp"

#include <iostream>

enum class InteractionType
{
    MOVE,
    ROTATE
};

class Menu
{
public:
    Menu() = default;

    void renderMenu(EllipsoidProperties& properties);
    float getMenuWidth();
    bool getMenuEnabled() { return mIsMenuEnabled; }
    InteractionType getInteractionType() { return mInteractionType; }

private:
    bool mIsMenuEnabled = false;
    InteractionType mInteractionType{};
};

inline void Menu::renderMenu(EllipsoidProperties& properties)
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

        if (ImGui::Combo("##InteractionType", reinterpret_cast<int*>(&mInteractionType),
            comboItems, IM_ARRAYSIZE(comboItems)))
        {
            isUIclicked = true;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("a", &properties.a, 0.0f, 10.0f);                      isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("b", &properties.b, 0.0f, 10.0f);                      isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("c", &properties.c, 0.1f, 10.0f);                      isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("scale", &properties.scaleFactor, 0.1f, 1.0f);         isUIclicked |= ImGui::IsItemActive();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("m", &properties.m, 0.1f, 10.0f);                      isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderInt("accuracy", &properties.accuracy, minFragmentSize, 128); isUIclicked |= ImGui::IsItemActive();
    }

    ImGui::End();
}

inline float Menu::getMenuWidth()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float menuWidth = windowSize.x * 0.2f;

    return menuWidth;
}