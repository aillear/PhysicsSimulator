#include "app.h"
#include "pathMgr.h"
#include "logger.h"
#include "eventSystem.h"
#include "renderSystem.h"

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
    GET_PathMgr.Init();
    GET_Logger.Init(Logger::INFO, "app.log", false);

    // system initialize
    GET_RenderSystem.Init();
    GET_EventSystem.Init();

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(SDL_QUIT, [this](SDL_Event& event) {
        this->running = false;
    });
}

void App::Run() {

    while (running) {
        GET_EventSystem.HandleEvent();
        GET_RenderSystem.Render();
    }
}

