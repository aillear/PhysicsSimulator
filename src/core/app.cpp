#include "app.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderSystem.h"
#include <SDL2_framerate.h>

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
    GET_Logger.Init(Logger::INFO, "app.log",
                    false); // when this is done, the logger will be usable

    // system initialize
    GET_RenderSystem.Init(800, 600, "physics demo");
    GET_EventSystem.Init();

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 60);

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(
        SDL_QUIT, [this](SDL_Event &event) { this->running = false; });
    LOG_INFO("QUIT event listener added.");
    LOG_INFO("App initialized.");
}

void App::Run() {

    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        GET_EventSystem.HandleEvent();
        // test here
        GET_RenderSystem.drawCommands.emplace_back(DrawCommand
            {true,
             0,
             DrawCommand::ShapeType::RECT,
             Transform({100, 100}, 100),
             {Vector2f(100, 100), Vector2f(200, 200)},
             nullptr,
             {255, 255, 0, 255}});

        GET_RenderSystem.Render();
        fpsc.EndFrame();
    }
}
