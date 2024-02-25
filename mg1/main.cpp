#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "math.hpp"
#include "equations.hpp"
#include "SDL.h"
#include <algorithm>

float a = 0.5f, b = 1.0f, c = 1.5f, scale = 3.0f, m = 4.0f;
int accuracy = 8;

void renderMenu()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float menuWidth = windowSize.x * 0.2f;

    ImGui::SetNextWindowPos(ImVec2(windowSize.x - menuWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(menuWidth, windowSize.y));

    if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Adjust Values");

        ImGui::SliderFloat("a", &a, 0.0f, 10.0f);
        ImGui::SliderFloat("b", &b, 0.0f, 10.0f);
        ImGui::SliderFloat("c", &c, 0.1f, 10.0f);
        ImGui::SliderFloat("scale", &scale, 0.0f, 10.0f);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("m", &m, 0.0f, 10.0f);
        ImGui::SliderInt("accuracy", &accuracy, 1, 20);
    }
    ImGui::End();
}

int main(int argc, char* argv[])
{
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);

    int windowWidth = 1280;
    int windowHeight = 720;

    SDL_Window* window = SDL_CreateWindow("mg1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    assert(window != nullptr);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    assert(renderer != nullptr);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool end = false;
    while (!end)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                end = true;
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        renderMenu();

        SDL_SetRenderDrawColor(renderer,
            static_cast<Uint8>(clearColor.x * 255),
            static_cast<Uint8>(clearColor.y * 255),
            static_cast<Uint8>(clearColor.z * 255),
            static_cast<Uint8>(clearColor.w * 255));

        SDL_RenderClear(renderer);

        ImGui::Render();

        SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1280, 720);
        Uint32* upixels;
        int pitch;
        SDL_LockTexture(texture, NULL, reinterpret_cast<void**>(&upixels), &pitch);

        Vec3 lightPos{0.0f, 0.0f, 10.0f};
        Vec3 lightColor{1.0f, 1.0f, 0.0f};
        float ambientStrength = 0.1f;
        Vec3 ambientColor = lightColor * ambientStrength;

        Vec3 diffuseColor = lightColor;
        Vec3 specularColor = Vec3(1.f);

        Vec3 viewPos = lightPos;

        for (int y = 0; y < windowHeight; ++y)
        {
            for (int x = 0; x < windowWidth; ++x)
            {
                float normalizedX = (static_cast<float>(x) / windowWidth - 0.5f) * scale;
                float normalizedY = (0.5f - static_cast<float>(y) / windowHeight) * scale;
                Vec4 color{0.0f, 0.0f, 0.0f, 1.0f};

                float z = elipsoidZ(normalizedX, normalizedY, a, b, c);
                if (!std::isnan(z))
                {
                    Vec3 point = {normalizedX, normalizedY, z};
                    Vec3 normal = normalVector(point.x, point.y, point.z, a, b, c);

                    Vec3 ambient = ambientColor;

                    Vec3 lightDir = normalize(lightPos - point);
                    float diff = std::max(dot(normal, lightDir), 0.f);
                    Vec3 diffuse = diff * diffuseColor;

                    Vec3 viewDir = normalize(viewPos - point);
                    Vec3 reflectDir = reflect(-lightDir, normal);
                    float spec = pow(std::max(dot(viewDir, reflectDir), 0.f), m);
                    Vec3 specular = spec * specularColor;

                    Vec3 finalColor = normal;

                    finalColor.x = std::clamp(pow(finalColor.x, m), 0.f, 1.f);
                    finalColor.y = std::clamp(pow(finalColor.y, m), 0.f, 1.f);
                    finalColor.z = std::clamp(pow(finalColor.z, m), 0.f, 1.f);

                    color = Vec4{finalColor.x, finalColor.y, finalColor.z, 1.f};
                }

                upixels[y * windowWidth + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888),
                    static_cast<Uint8>(color.x * 255), static_cast<Uint8>(color.y * 255), static_cast<Uint8>(color.z * 255), static_cast<Uint8>(color.w * 255));
            }
        }


        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_DestroyTexture(texture);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(renderer);
    }


    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}