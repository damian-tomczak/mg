#include "renderer.hpp"

#include <thread>

Menu menu;

int main(int argc, char* argv[])
{
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);

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

    ImVec4 clearColor{0.45f, 0.55f, 0.60f, 1.00f};

    SDL_Texture* previousTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
    SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

    AdaptiveRenderer adaptiveRenderer;

    EllipsoidProperties properties;

    int accuracyCounter = properties.accuracy;

    bool isDragging = false;
    int lastMouseX, lastMouseY;

    std::thread ellipsoidDrawing = std::thread(&AdaptiveRenderer::drawElipsoid, &adaptiveRenderer, newTexture, accuracyCounter, properties);

    bool end = false;
    while (!end)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type)
            {
            case SDL_QUIT:
                end = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = true;
                    SDL_GetMouseState(&lastMouseX, &lastMouseY);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = false;
                }
                break;
            case SDL_MOUSEMOTION:
                if (isDragging)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    if (menu.getMenuEnabled() && (mouseX >= windowWidth - menu.getMenuWidth()))
                    {
                        isDragging = false;
                        break;
                    }

                    int dx = mouseX - lastMouseX;
                    int dy = mouseY - lastMouseY;

                    // TODO: deltaTime

                    InteractionType interactionType = menu.getInteractionType();
                    switch(interactionType)
                    {
                    case InteractionType::MOVE:
                        properties.position.x += dx * mouseSensitivity;
                        properties.position.y += -dy * mouseSensitivity;
                        break;
                    case InteractionType::ROTATE:
                        properties.rotation.x += dx * mouseSensitivity;
                        properties.rotation.y += -dy * mouseSensitivity;
                        break;
                    default:
                        assert(false);
                    }

                    lastMouseX = mouseX;
                    lastMouseY = mouseY;
                }
                break;
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        menu.renderMenu(properties);

        SDL_SetRenderDrawColor(renderer,
            static_cast<Uint8>(clearColor.x * 255),
            static_cast<Uint8>(clearColor.y * 255),
            static_cast<Uint8>(clearColor.z * 255),
            static_cast<Uint8>(clearColor.w * 255));

        SDL_RenderClear(renderer);

        ImGui::Render();

        bool reRender = adaptiveRenderer.shouldReRender(properties);

        if (reRender || (accuracyCounter >= minFragmentSize) || isUIclicked)
        {
            if (reRender || isUIclicked)
            {
                accuracyCounter = properties.accuracy;
            }

            if (isFinished)
            {
                ellipsoidDrawing.join();
                isFinished = false;

                SDL_SetRenderTarget(renderer, previousTexture);

                SDL_RenderCopy(renderer, newTexture, nullptr, nullptr);

                SDL_SetRenderTarget(renderer, nullptr);

                ellipsoidDrawing = std::thread(&AdaptiveRenderer::drawElipsoid, &adaptiveRenderer, newTexture, accuracyCounter, properties);
                accuracyCounter--;
            }

        }

        SDL_RenderCopy(renderer, previousTexture, nullptr, nullptr);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(renderer);
    }

    ellipsoidDrawing.join();

    SDL_DestroyTexture(previousTexture);
    SDL_DestroyTexture(newTexture);

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}