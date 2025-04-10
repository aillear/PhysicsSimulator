#include "test.h"
#include "UIButton.h"
#include "UILabelReader.h"
#include "UIMgr.h"
#include "UIPanel.h"
#include "app.h"
#include "eventSystem.h"
#include "logger.h"
#include "material.h"
#include "physicsSystem.h"
#include "renderSystem.h"
#include <SDL3_framerate.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/vec2.hpp>
#include <memory>

void SomeCustomLogicHere() {
    auto panel =
        std::make_shared<UIPanel>(glm::vec2{0, 100}, glm::vec2{400, 600});

    panel->SetColor({40, 44, 52, 255});
    panel->SetBarColor({33, 37, 43, 255});
    panel->SetBarHeight(40.0f);
    panel->SetName("panel1");
    panel->SetEnabled(true);

    auto button2 =
        std::make_shared<UIButton>(glm::vec2{0, 0}, glm::vec2{30, 30});
    button2->SetColor({157, 42, 18, 255});

    button2->SetColorHover({229, 74, 41, 255});
    button2->SetColorPressed({233, 125, 102, 255});
    button2->SetCallBack([panel](SDL_Event &event) {
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
    label1->SetFColor({1, 1, 1, 1});
    label1->SetName("frame time reader");
    auto &readerRender = GET_App.fpsc;
    label1->AddReader([&readerRender]() {
        return std::format("{:.2f}ms", readerRender.GetLastFrameTime());
    });
    label1->SetAlignMent(UIComponent::TextAlign::END,
                         UIComponent::TextAlign::START, {0, 0}, {5, 5});
    label1->SetEnabled(true);

    auto label2 = std::make_shared<UILabelReader>();
    label2->SetFColor({1, 1, 1, 1});
    label2->SetName("frame rate reader");
    label2->AddReader([&readerRender]() {
        return std::format("{}FPS", readerRender.GetFPS());
    });
    label2->SetAlignMent(UIComponent::TextAlign::END,
                         UIComponent::TextAlign::START, {0, FONT_SIZE + 5},
                         {5, 5});
    label2->SetEnabled(true);

    auto label3 = std::make_shared<UILabelReader>();
    label3->SetFColor({1, 1, 1, 1});
    label3->SetName("frame time reader");
    auto &reader = GET_PhysicsSystem.fpsc;
    label3->AddReader([&reader]() {
        return std::format("{:.2f}ms", reader.GetLastFrameTime());
    });
    label3->SetAlignMent(UIComponent::TextAlign::START,
                         UIComponent::TextAlign::START, {0, 0}, {5, 5});
    label3->SetEnabled(true);

    auto label4 = std::make_shared<UILabelReader>();
    label4->SetFColor({1, 1, 1, 1});
    label4->SetName("frame time reader");
    label4->AddReader(
        [&reader]() { return std::format("{}FPS", reader.GetFPS()); });
    label4->SetAlignMent(UIComponent::TextAlign::START,
                         UIComponent::TextAlign::START, {0, FONT_SIZE + 5},
                         {5, 5});
    label4->SetEnabled(true);

    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN, [panel](SDL_Event &event) {
            if (event.button.button != SDL_BUTTON_RIGHT)
                return;
            if (!panel->GetEnabled()) {
                panel->SetEnabled(true);
                panel->SetRelativePos({0, panel->GetBarHeight()});
            }
        });

    GET_EventSystem.AddEventListener(SDL_EVENT_KEY_DOWN,
                                     [button2](SDL_Event &event) {
                                         if (event.key.key != SDLK_SPACE)
                                             return;
                                         GET_UIMgr.RemoveUIComponent(button2);
                                     });

    GET_UIMgr.AddUIComponent(panel);
    // GET_UIMgr.AddUIComponent(button, panel);
    GET_UIMgr.AddUIComponent(button2, panel);
    GET_UIMgr.AddUIComponent(label, panel);
    GET_UIMgr.AddUIComponent(label1, panel);
    GET_UIMgr.AddUIComponent(label2, panel);
    GET_UIMgr.AddUIComponent(label3, panel);
    GET_UIMgr.AddUIComponent(label4, panel);

    panel->SetBarAlignMent(button2, TextAlign::END, TextAlign::CENTER, {0, 0},
                           {5, 0});
    panel->SetBarAlignMent(label, TextAlign::START, TextAlign::CENTER, {0, 0},
                           {10, 0});
}

void SomeCustomLogicPHere() {
    Material m{1, 1, 0.5};
    // for (int i = 0; i < 100; i++) {
    //     GET_PhysicsSystem.AddObject(
    //         CreateCircleBody(20, {(i % 100) * 100, (i / 100) * 100}, m));
    // }

    // for (int i = 0; i < 100; i++) {
    //     GET_PhysicsSystem.AddObject(
    //         CreateBoxBody({20, 20}, {(i % 100) * 100, (i / 100) * 100 + 1000}, m));
    // }
}