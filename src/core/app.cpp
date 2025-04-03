#include "app.h"
#include "UIBotton.h"
#include "UIMgr.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderSystem.h"
#include <SDL3_framerate.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/vec2.hpp>

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
    GET_UIMgr.Init();

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 90);

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(
        SDL_EVENT_QUIT, [this](SDL_Event &event) { this->running = false; });
    LOG_INFO("QUIT event listener added.");
    LOG_INFO("App initialized.");
    LOG_INFO("Running...");
    LOG_INFO("==========================");
    LOG_INFO("");
}

void App::Run() {

    auto button = std::make_shared<UIButton>(
        glm::vec2{0, 0}, glm::vec2{200, 100});
    button->SetColor({0, 0, 255, 255});
    button->SetColorHover({0, 255, 0, 255});
    button->SetColorPressed({255, 0, 0, 255});
    button->SetCallBack([](SDL_Event &event) { LOG_INFO("Button1 clicked!"); });
    button->SetName("button1");
    button->SetEnabled(true);

    auto button1 = std::make_shared<UIButton>(
        glm::vec2{0,75}, glm::vec2{200, 100});
    button1->SetColor({0, 0, 255, 255});
    button1->SetColorHover({0, 255, 0, 255});
    button1->SetColorPressed({255, 0, 0, 255});
    button1->SetCallBack([](SDL_Event &event) { LOG_INFO("Button2 clicked!"); });
    button1->SetName("button2");
    button1->SetEnabled(true);


    GET_UIMgr.AddUIComponent(button);
    GET_UIMgr.AddUIComponent(button1);
    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        GET_EventSystem.HandleEvent();

        GET_UIMgr.Update(fpsc.GetLastFrameTime());
        GET_RenderSystem.Render();
        fpsc.EndFrame();
    }
    Destory();
}

void App::Destory() {
    // GET_RenderSystem.Destroy();
    GET_UIMgr.Destroy();

    // GET_EventSystem.Destroy();
    // GET_Logger.Destroy();
    // GET_PathMgr.Destroy();
    LOG_INFO("App destroyed.");
}
