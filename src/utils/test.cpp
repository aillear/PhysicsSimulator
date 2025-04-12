#include "test.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "UIButton.h"
#include "UIComponent.h"
#include "UILabelReader.h"
#include "UIMgr.h"
#include "UIPanel.h"
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
#include "rigidbody.h"
#include <SDL3_framerate.h>
#include <format>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <memory>

void SomeCustomLogicHere() {
    auto panel = std::make_shared<UIPanel>(glm::vec2{400, 600});

    panel->SetColor({40, 44, 52, 255});
    panel->SetBarColor({33, 37, 43, 255});
    panel->SetBarHeight(40.0f);
    panel->SetName("panel1");
    panel->SetEnabled(false);

    auto button2 = std::make_shared<UIButton>(glm::vec2{30, 30});
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

    auto label5 = std::make_shared<UILabelReader>();
    label5->SetFColor({1, 1, 1, 1});
    label5->SetName("cameraReader");
    auto &camera = GET_RenderSystem.GetCamera();
    label5->AddReader([&camera]() {
        return std::format("camera zoom: {}, location: {}", camera.getZoom(),
                           camera.GetPosition());
    });
    label5->SetAlignMent(UIComponent::TextAlign::START,
                         UIComponent::TextAlign::START,
                         {0, (FONT_SIZE + 5) * 2}, {5, 5});
    label5->SetEnabled(true);

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
    // GET_UIMgr.AddUIComponent(button, panel);
    GET_UIMgr.AddUIComponent(button2, panel);
    GET_UIMgr.AddUIComponent(label, panel);
    GET_UIMgr.AddUIComponent(label1, panel);
    GET_UIMgr.AddUIComponent(label2, panel);
    GET_UIMgr.AddUIComponent(label3, panel);
    GET_UIMgr.AddUIComponent(label4, panel);
    GET_UIMgr.AddUIComponent(label5, panel);

    auto exitPanel = std::make_shared<UIPanel>(glm::vec2{320, 240});
    exitPanel->SetName("exit menu");
    exitPanel->SetBarHeight(40.0f);
    exitPanel->SetAlignMent(TextAlign::CENTER, TextAlign::CENTER);
    exitPanel->SetBarColor({33, 37, 43, 255});
    exitPanel->SetColor({40, 44, 52, 255});
    exitPanel->SetEnabled(false);

    auto button = std::make_shared<UIButton>(glm::vec2{30, 30});
    button->SetName("button");
    button->SetColor({157, 42, 18, 255});
    button->SetColorHover({229, 74, 41, 255});
    button->SetColorPressed({233, 125, 102, 255});
    button->SetCallBack(
        [exitPanel](SDL_Event &event) { exitPanel->SetEnabled(false); });
    button->SetEnabled(true);

    auto exitButton = std::make_shared<UIButton>(glm::vec2{100, 50});
    exitButton->SetName("EXIT BUTTON");
    exitButton->SetColor({157, 42, 18, 255});

    exitButton->SetColorHover({229, 74, 41, 255});
    exitButton->SetColorPressed({233, 125, 102, 255});
    exitButton->SetAlignMent(UIComponent::TextAlign::CENTER,
                             UIComponent::TextAlign::CENTER);
    exitButton->SetCallBack([](SDL_Event &) {
        SDL_Event e;
        e.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&e);
    });
    exitButton->SetEnabled(true);

    auto exitlabel = std::make_shared<UILabel>();
    exitlabel->ChangeText("menu");
    exitlabel->SetColor({0, 255, 255, 255});
    exitlabel->SetName("exitlabel1");
    exitlabel->SetEnabled(true);

    GET_EventSystem.AddEventListener(SDL_EVENT_KEY_DOWN,
                                     [exitPanel](SDL_Event &e) {
                                         if (e.key.key != SDLK_ESCAPE)
                                             return;
                                         exitPanel->SetEnabled(true);
                                     });

    panel->SetBarAlignMent(button2, TextAlign::END, TextAlign::CENTER, {0, 0},
                           {5, 0});
    panel->SetBarAlignMent(label, TextAlign::START, TextAlign::CENTER, {0, 0},
                           {10, 0});

    exitPanel->SetBarAlignMent(button, TextAlign::END, TextAlign::CENTER,
                               {0, 0}, {5, 0});
    exitPanel->SetBarAlignMent(exitlabel, TextAlign::START, TextAlign::CENTER,
                               {0, 0}, {10, 0});

    GET_UIMgr.AddUIComponent(exitPanel);
    GET_UIMgr.AddUIComponent(exitButton, exitPanel);
    GET_UIMgr.AddUIComponent(button, exitPanel);
    GET_UIMgr.AddUIComponent(exitlabel, exitPanel);
}

void SomeCustomLogicPHere() {
    Material m{1, 1, 0.5};
    // for (int i = 0; i < 10; i++) {
    //     auto circle = std::make_shared<CircleBody>(m, glm::vec2{i * 40, i *
    //     40}, 20); circle->SetFColor(RandomFColor());
    //     GET_PhysicsSystem.AddObject(circle);
    // }

    for (int i = 0; i < 20; i++) {
        auto pos = RandomPos({20, 20}, {780, 580});
        int type = RandomInt(0, 2);
        std::shared_ptr<RigidBody> obj;
        if (type == 0)
            obj = std::make_shared<CircleBody>(m, pos, 20);
        else
            obj = std::make_shared<BoxBody>(m, pos, glm::vec2{30, 20});

        if (i != 0) {
            obj->SetIsStatic(RandomBool());
            obj->SetFColor(RandomFColor());
            if (obj->GetIsStatic()) {
                obj->SetFColorBoundry({1, 0, 0, 1});
            }
        }
        GET_PhysicsSystem.AddObject(obj);
    }
}

void SomeCustomLogicPAHere() {
    auto children = GET_PhysicsSystem.GetRootNode()->GetChildren();
    const float dt = GET_PhysicsSystem.GetTargetDt();

    glm::vec2 dir = {0, 0};

    if (KeyState(SDL_SCANCODE_W))
        dir.y--;
    if (KeyState(SDL_SCANCODE_S))
        dir.y++;
    if (KeyState(SDL_SCANCODE_A))
        dir.x--;
    if (KeyState(SDL_SCANCODE_D))
        dir.x++;

    float forceMagnitude = 120000.0f;
    if (dir.x != 0 || dir.y != 0) {
        auto firstObj = static_cast<RigidBody *>(children.begin()->get());
        if (firstObj == nullptr)
            return;
        firstObj->AddForce(forceMagnitude * glm::normalize(dir));
    }
}