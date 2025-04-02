#include "app.h"
#include "conversion.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderSystem.h"
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
            DrawCommand cmd(ShapeType::CIRCLE, false);
            cmd.GetBase().circle.center = {500, 500};
            cmd.GetBase().circle.radius = 100;
            cmd.GetBase().color = {255, 0, 0, 255};
            // test here
            GET_RenderSystem.AddUIDrawCommand(std::move(cmd));
        }

        GET_RenderSystem.Render();
        fpsc.EndFrame();
    }
}
