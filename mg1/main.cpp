#include "rendering.hpp"

bool shouldReRender()
{
    static RendererValues prev = {a, b, c, scaleObj, m, startingAccuracy, objPos};

    bool changed = false;

    if (std::fabs(a - prev.a) > floatDiff ||
        std::fabs(c - prev.c) > floatDiff ||
        std::fabs(m - prev.m) > floatDiff ||

        std::fabs(objPos.x - prev.objPos.x) > floatDiff ||
        std::fabs(objPos.y - prev.objPos.y) > floatDiff ||
        std::fabs(objPos.z - prev.objPos.z) > floatDiff ||

        std::fabs(b - prev.b) > floatDiff ||
        std::fabs(scaleObj - prev.scaleObj) > floatDiff ||
        startingAccuracy != prev.accuracy
        )
    {
        changed = true;
    }

    prev.a = a;
    prev.b = b;
    prev.c = c;
    prev.scaleObj = scaleObj;
    prev.m = m;
    prev.accuracy = startingAccuracy;
    prev.objPos = objPos;

    return changed;
}

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

    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);
    Uint32* pixels;
    int pitch;

    bool isDragging = false;
    int lastMouseX, lastMouseY;

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

                    if (isMenuEnabled && (mouseX >= windowWidth - Menu::getMenuWidth()))
                    {
                        isDragging = false;
                        break;
                    }

                    int dx = mouseX - lastMouseX;
                    int dy = mouseY - lastMouseY;

                    // TODO: deltaTime
                    objPos.x += dx * mouseSensitivity;
                    objPos.y += -dy * mouseSensitivity;

                    lastMouseX = mouseX;
                    lastMouseY = mouseY;
                }
                break;
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        Menu::renderMenu();

        SDL_SetRenderDrawColor(renderer,
            static_cast<Uint8>(clearColor.x * 255),
            static_cast<Uint8>(clearColor.y * 255),
            static_cast<Uint8>(clearColor.z * 255),
            static_cast<Uint8>(clearColor.w * 255));

        SDL_RenderClear(renderer);

        ImGui::Render();

        bool reRender = shouldReRender();

        if (reRender || (accuracy > minFragmentSize) || isUIclicked)
        {
            SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixels), &pitch);

            if (reRender || isUIclicked)
            {
                accuracy = startingAccuracy;
            }

            int totalBytes = pitch * windowHeight;
            memset(pixels, 0, totalBytes);

            processFragments(0, 0, windowWidth, windowHeight, accuracy, pixels);

            SDL_UnlockTexture(texture);

            accuracy--;
        }

        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}