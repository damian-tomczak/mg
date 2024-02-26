#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "globals.hpp"

#include <iostream>

bool checkIfGlobalsChanged()
{
    static PreviousValues prev = { a, b, c, scaleObj, m, startingAccuracy };

    bool changed = false;

    if (std::fabs(a - prev.a) > floatDiff || std::fabs(b - prev.b) > floatDiff ||
        std::fabs(c - prev.c) > floatDiff || std::fabs(scaleObj - prev.scaleObj) > floatDiff ||
        std::fabs(m - prev.m) > floatDiff || startingAccuracy != prev.accuracy)
    {
        changed = true;
    }

    prev.a = a;
    prev.b = b;
    prev.c = c;
    prev.scaleObj = scaleObj;
    prev.m = m;
    prev.accuracy = startingAccuracy;

    return changed;
}

void renderMenu()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float menuWidth = windowSize.x * 0.2f;

    ImGui::SetNextWindowPos(ImVec2(windowSize.x - menuWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(menuWidth, windowSize.y));

    if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Adjust Values");

        isUIclicked = false;

        ImGui::SliderFloat("a", &a, 0.0f, 10.0f);                              isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("b", &b, 0.0f, 10.0f);                              isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("c", &c, 0.1f, 10.0f);                              isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderFloat("scaleObj", &scaleObj, 0.0f, 10.0f);                isUIclicked |= ImGui::IsItemActive();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("m", &m, 0.1f, 10.0f);                              isUIclicked |= ImGui::IsItemActive();
        ImGui::SliderInt("accuracy", &startingAccuracy, minFragmentSize, 128); isUIclicked |= ImGui::IsItemActive();
    }
    ImGui::End();
}