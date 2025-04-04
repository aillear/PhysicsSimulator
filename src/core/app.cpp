#include "app.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "UIBotton.h"
#include "UIMgr.h"
#include "UIPanel.h"
#include "conversion.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderSystem.h"
#include <SDL3_framerate.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/vec2.hpp>
#include <memory>

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
    GET_RenderSystem.Init(1'000'000, 800, 600, {50,56,66,255}, "physics demo");
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
    auto panel = std::make_shared<UIPanel>(
        glm::vec2{0, 100}, glm::vec2{400, 600});

    panel->SetColor(ToFColor({40,44,52,255}));
    panel->SetBarColor(ToFColor({33,37,43, 255}));
    panel->SetBarHeight(40.0f);
    panel->SetName("panel1");
    panel->SetEnabled(true);


    auto button = std::make_shared<UIButton>(
        glm::vec2{0, 0}, glm::vec2{200, 100});
    button->SetColor({0, 0, 255, 255});
    button->SetColorHover({0, 255, 0, 255});
    button->SetColorPressed({255, 0, 0, 255});
    button->SetCallBack([](SDL_Event &event) { LOG_INFO("Button1 clicked!"); });
    button->SetName("button1");
    button->SetEnabled(true);

    auto button1 = std::make_shared<UIButton>(
        glm::vec2{0,150}, glm::vec2{200, 100});
    button1->SetColor({0, 0, 255, 255});
    button1->SetColorHover({0, 255, 0, 255});
    button1->SetColorPressed({255, 0, 0, 255});
    button1->SetCallBack([&panel](SDL_Event &event) { LOG_INFO("Button2 clicked!");
        panel->SetEnabled(false);
    });
    button1->SetName("button2");
    button1->SetEnabled(true);

    GET_EventSystem.AddEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN, [&panel](SDL_Event &event) {
        if (event.button.button != SDL_BUTTON_RIGHT) return;
        if (!panel->GetEnabled()) panel->SetEnabled(true);
    });

    GET_UIMgr.AddUIComponent(panel);
    GET_UIMgr.AddUIComponent(button, panel);
    GET_UIMgr.AddUIComponent(button1, panel);

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
