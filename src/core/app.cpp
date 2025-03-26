#include "app.h"
#include "pathMgr.h"
#include "logger.h"
#include "eventSystem.h"
#include "renderSystem.h"

App& App::Instance() {
    static App instance;
    return instance;
}

void App::Init(int argc, char* argv[]) {
    // set running to true.
    running = true;
    // utils initialize
    PathMgr::Instance().Init();
    Logger::Instance().Init(Logger::INFO, "app.log", false);

    // system initialize
    RenderSystem::Instance().Init();
}

void App::Run() {

    while (running) {
        GET_EventSystem.HandleEvent();
        GET_RenderSystem.Render();
    }
}

