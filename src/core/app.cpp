#include "app.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "UIButton.h"
#include "UIComponent.h"
#include "UILabelReader.h"
#include "UIMgr.h"
#include "UIPanel.h"
#include "conversion.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderSystem.h"
#include <SDL3_framerate.h>
#include <cstdlib>
#include <format>
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
    RenderSystemIniter RenderSystemIniter;

    if (!GET_RenderSystem.Init(RenderSystemIniter)) {
        LOG_ERROR("Render system initialize failed.");
        exit(1);
    }

    GET_EventSystem.Init();
    GET_UIMgr.Init();

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 90);

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(
        SDL_EVENT_QUIT, [this](SDL_Event &event) { this->running = false; });
    LOG_INFO("App initialized.");
    LOG_INFO("Running...");
    LOG_INFO("==========================");
}

void App::Run() {
    auto panel =
        std::make_shared<UIPanel>(glm::vec2{0, 100}, glm::vec2{400, 600});

    panel->SetColor({40, 44, 52, 255});
    panel->SetBarColor(ToFColor({33, 37, 43, 255}));
    panel->SetBarHeight(40.0f);
    panel->SetName("panel1");
    panel->SetEnabled(true);


    auto button2 =
        std::make_shared<UIButton>(glm::vec2{0, 0}, glm::vec2{30, 30});
    button2->SetColor({157, 42, 18, 255});

    button2->SetColorHover({229, 74, 41, 255});
    button2->SetColorPressed({233, 125, 102, 255});
    button2->SetCallBack([&panel](SDL_Event &event) {
        LOG_INFO("Button2 clicked!");
        panel->SetEnabled(false);
    });
    button2->SetName("button2");
    button2->SetEnabled(true);

    auto label = std::make_shared<UILabel>();
    label->ChangeText("Information");
    label->SetColor({0, 255, 255, 255});
    label->SetName("label1");
    label->SetEnabled(true);

    auto label1 = std::make_shared<UILabelReader>();
    label1->SetFColor({1,1,1,1});
    label1->SetName("frame time reader");
    label1->AddReader([this](){return std::format("{:.2f}ms", this->fpsc.GetLastFrameTime());});
    label1->SetAlignMent(UIComponent::TextAlign::END, UIComponent::TextAlign::START, {0, 0}, {5, 5});
    label1->SetEnabled(true);

    auto label2 = std::make_shared<UILabelReader>();
    label2->SetFColor({1,1,1,1});
    label2->SetName("frame time reader");
    label2->AddReader([this](){return std::format("{}FPS", this->fpsc.GetFPS());});
    label2->SetAlignMent(UIComponent::TextAlign::END, UIComponent::TextAlign::START, {0, FONT_SIZE+5}, {5, 5});
    label2->SetEnabled(true);

    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN, [&panel](SDL_Event &event) {
            if (event.button.button != SDL_BUTTON_RIGHT)
                return;
            if (!panel->GetEnabled()) {
                panel->SetEnabled(true);
                panel->SetRelativePos({0, panel->GetBarHeight()});
            }
        });

        GET_UIMgr.AddUIComponent(panel);
        // GET_UIMgr.AddUIComponent(button, panel);
        GET_UIMgr.AddUIComponent(button2, panel);
        GET_UIMgr.AddUIComponent(label, panel);
        GET_UIMgr.AddUIComponent(label1, panel);
        GET_UIMgr.AddUIComponent(label2, panel);
        panel->SetBarAlignMent(button2, TextAlign::END, TextAlign::CENTER, {0, 0}, {5, 0});
        panel->SetBarAlignMent(label, TextAlign::START, TextAlign::CENTER, {0, 0}, {10, 0});
        
    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        GET_EventSystem.HandleEvent();

        GET_UIMgr.Update(fpsc.GetLastFrameTime());
        GET_RenderSystem.Render();
        fpsc.EndFrame();
    }
    Destroy();
}

void App::Destroy() {
    GET_RenderSystem.Destroy();
    GET_UIMgr.Destroy();
    GET_RenderSystem.Destroy();
    GET_EventSystem.Destroy();
    GET_Logger.Destroy();
    GET_PathMgr.Destroy();
    LOG_INFO("App destroyed.");
}
