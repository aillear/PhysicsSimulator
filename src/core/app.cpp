#include "app.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderSystem.h"
#include <random>
#include <glm/vec2.hpp>
#include <SDL3_framerate.h>
#include <glm/ext/vector_float2.hpp>

App &App::Instance() {
    static App instance;
    return instance;
}
App::App() : running(false) { ; }

void App::Init(int argc, char *argv[]) {
    // set running to true.
    running = true;
    // utils initialize
    GET_PathMgr.Init(); // PathMgr should be initialized first
    GET_Logger.Init(Logger::DEBUG, "app.log",
                    false); // when this is done, the logger will be usable

    // system initialize
    GET_RenderSystem.Init(1'000'000, 800, 600, "physics demo");
    GET_EventSystem.Init();

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 120);

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(
        SDL_EVENT_QUIT, [this](SDL_Event &event) { this->running = false; });
    LOG_INFO("QUIT event listener added.");
    LOG_INFO("App initialized.");
    LOG_INFO("Running...");
}

void App::Run() {

    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        GET_EventSystem.HandleEvent();

        for (int i = 0; i < 100; i++) {
            // test here
            GET_RenderSystem.AddUIDrawCommand({
                0,
                DrawCommand::ShapeType::RECT,
                {glm::vec2(i*10, i*10), glm::vec2(i*20, 20*i)},
                {},
                nullptr,
                {1, 1, 0, 1},
            });
        }

        GET_RenderSystem.Render();
        fpsc.EndFrame();
    }
}
