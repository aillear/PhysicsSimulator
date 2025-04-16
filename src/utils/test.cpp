#include "test.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_scancode.h"
#include "UIButton.h"
#include "UIComponent.h"
#include "UILabelReader.h"
#include "UIMgr.h"
#include "UIPanelLines.h"
#include "app.h"
#include "boxBody.h"
#include "circleBody.h"
#include "conversion.h"
#include "eventSystem.h"
#include "inputSystem.h"
#include "logger.h"
#include "material.h"
#include "physicsSystem.h"
#include "renderSystem.h"
#include <SDL3_framerate.h>
#include <format>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <utility>

void SomeCustomLogicHere() {

    auto panel = std::make_shared<UIPanelLines>(glm::vec2{500, 800}, 40.0f,
                                                "信息面板", 5.0f);
    panel->SetColor({40, 44, 52, 255});
    panel->SetBarColor({33, 37, 43, 255});
    panel->SetEnabled(false);

    auto bt = panel->GetCloseButton();
    bt->SetColor({157, 42, 18, 255});
    bt->SetColorHover({229, 74, 41, 255});
    bt->SetColorPressed({233, 125, 102, 255});

    auto lb = panel->GetTitleLabel();
    lb->SetFColor({0, 255, 255, 255});

    auto label = std::make_shared<UILabelReader>();
    auto &readerRender = GET_App.fpsc;
    label->AddReader([&readerRender]() {
        return std::format("渲染帧生成时: {:.2f}ms",
                           readerRender.GetLastFrameTime());
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    auto label2 = std::make_shared<UILabelReader>();
    label2->AddReader([&readerRender]() {
        return std::format("渲染帧率: {}FPS", readerRender.GetFPS());
    });
    panel->AddNewContent(std::move(label2), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    auto label3 = std::make_shared<UILabelReader>();
    label3->AddReader([]() {
        return std::format("物理帧生成时： {:.2f}ms",
                           GET_PhysicsSystem.GetFrameTime());
    });
    panel->AddNewContent(std::move(label3), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    auto label4 = std::make_shared<UILabelReader>();
    label4->AddReader([]() {
        return std::format("物理帧率: {}FPS", GET_PhysicsSystem.GetFPS());
    });
    panel->AddNewContent(std::move(label4), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    auto label5 = std::make_shared<UILabelReader>();
    label5->AddReader([]() {
        return std::format("Body count: {}", GET_PhysicsSystem.GetBodyCount());
    });
    panel->AddNewContent(std::move(label5), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN, [panel](SDL_Event &event) {
            if (event.button.button != SDL_BUTTON_RIGHT)
                return;
            if (!panel->GetEnabled()) {
                panel->SetEnabled(true);
                panel->SetRelativePos({0, panel->GetBarHeight()});
            }
        });

    GET_UIMgr.AddUIComponent(panel);

    GET_EventSystem.AddEventListener(SDL_EVENT_KEY_DOWN, [](SDL_Event &e) {
        if (e.key.key != SDLK_ESCAPE)
            return;
        SDL_Event quitEvent;
        quitEvent.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quitEvent);
    });
}

void SomeCustomLogicPHere() {
    Material m{1, 1, 0.5};
    
    auto ground = std::make_shared<BoxBody>(m, glm::vec2(2000, 20));
    ground->MoveTo({0, 800});
    ground->SetFColorBoundry({0, 0, 0, 1});
    ground->SetColor({77, 120, 204, 255});
    ground->SetIsStatic(true);
    ground->SetName("ground");
    GET_PhysicsSystem.AddObject(ground);

    auto wallL = std::make_shared<BoxBody>(m, glm::vec2(20, 1000));
    wallL->MoveTo({-400, 400});
    wallL->SetFColorBoundry({0, 0, 0, 1});
    wallL->SetColor({77, 120, 204, 255});
    wallL->SetIsStatic(true);
    wallL->SetName("wall1");
    GET_PhysicsSystem.AddObject(wallL);

    auto wallR = std::make_shared<BoxBody>(m, glm::vec2(20, 1000));
    wallR->MoveTo({400, 400});
    wallR->SetFColorBoundry({0, 0, 0, 1});
    wallR->SetColor({77, 120, 204, 255});
    wallR->SetIsStatic(true);
    wallR->SetName("wall2");
    GET_PhysicsSystem.AddObject(wallR);
}

void SomeCustomLogicPAHere() {
    auto children = GET_PhysicsSystem.GetRootNode()->GetChildren();

    Material m{0.5, 0.8, 0.5};
    static int counter = 0;
    if (PKeyDown(SDL_SCANCODE_Q)) {
        auto obj =
            std::make_shared<BoxBody>(m, RandomPos({50, 50}, {100, 100}));
        obj->SetFColor(RandomFColor());
        obj->MoveTo(SCREEN2WORLD(MousePos));
        obj->SetName(std::format("the {}'th object", counter++));
        obj->SetIsStatic(false);
        GET_PhysicsSystem.AddObject(obj);
    }

    if (PKeyDown(SDL_SCANCODE_E)) {
        auto obj = std::make_shared<CircleBody>(m, RandomFloat(25, 50));
        obj->SetFColor(RandomFColor());
        obj->MoveTo(SCREEN2WORLD(MousePos));
        obj->SetName(std::format("the {}'th object", counter++));
        obj->SetIsStatic(false);
        GET_PhysicsSystem.AddObject(obj);
    }
}