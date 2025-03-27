#include "renderSystem.h"
#include "eventSystem.h"
#include "logger.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keycode.h>

RenderSystem& RenderSystem::Instance() {
    static RenderSystem instance;
    return instance;
}

RenderSystem::RenderSystem()
    : window(nullptr), renderer(nullptr), windowSize(0, 0),
      camera({0.0f, 0.0f}, 1) {
    ;
}

RenderSystem::~RenderSystem() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    LOG_INFO("SDL renderer and window has been destroied successfully.");
    SDL_Quit();
    LOG_INFO("SDL quit");
}

bool RenderSystem::Init(int width, int height, const std::string &windowName) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        F_LOG_ERROR("Fail to init SDL: {}", SDL_GetError());
        return false;
    }
    LOG_INFO("SDL initialized");

    window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        F_LOG_ERROR("Fail to create SDL window: {}", SDL_GetError());
        return false;
    }
    F_LOG_INFO("SDL window {} created successfully.", windowName);
    windowSize = {width, height};
    F_LOG_INFO("window size: {}.", windowSize);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        F_LOG_ERROR("Fail to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    LOG_INFO("SDL renderer created successfully.");

    GET_EventSystem.AddEventListener(SDL_WINDOWEVENT, [this](SDL_Event &event) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            this->SetWindowSize({event.window.data1, event.window.data2});
        }
    });
    return true;
}

void RenderSystem::SetWindowSize(Vector2i size) {
    windowSize = size;
    F_LOG_INFO("resize window to: {}.", windowSize);
}

Vector2i RenderSystem::PosWorld2Screen(Vector2f worldPos) {
    Vector2f offset(camera.scale*windowSize.x, camera.scale*windowSize.y);
    Vector2f temp = worldPos - camera.position + offset;
    return Vector2i(temp.x, temp.y);
}


void RenderSystem::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}