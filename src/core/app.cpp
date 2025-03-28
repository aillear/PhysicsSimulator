#include "app.h"
#include "pathMgr.h"
#include "logger.h"
#include "eventSystem.h"
#include "renderSystem.h"
#include <SDL2_framerate.h>

App& App::Instance() {
    static App instance;
    return instance;
}
App::App() : running(false) {
    ;   
}

void App::Init(int argc, char* argv[]) {
    // set running to true.
    running = true;
    // utils initialize
    GET_PathMgr.Init();  // PathMgr should be initialized first
    GET_Logger.Init(Logger::INFO, "app.log", false);  // when this is done, the logger will be usable

    // system initialize
    GET_RenderSystem.Init(800, 600, "physics demo");
    GET_EventSystem.Init();

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 60);

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(SDL_QUIT, [this](SDL_Event& event) {
        this->running = false;
    });
    LOG_INFO("QUIT event listener added.");
    LOG_INFO("App initialized.");
}

void App::Run() {

    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        GET_EventSystem.HandleEvent();
        GET_RenderSystem.Render();
        fpsc.EndFrame();
    }
}

