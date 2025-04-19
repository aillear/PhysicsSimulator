#include "test.h"
#include "UIComponent.h"
#include "UILabelReader.h"
#include "UIMgr.h"
#include "UIPanelLines.h"
#include "app.h"
#include "boxBody.h"
#include "circleBody.h"
#include "configs.h"
#include "conversion.h"
#include "inputSystem.h"
#include "physicsSystem.h"
#include "renderSystem.h"
#include "link.h"
#include "rope.h"
#include "cord.h"
#include "spring.h"




void InitApp() {

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

    label = std::make_shared<UILabelReader>();
    label->AddReader([&readerRender]() {
        return std::format("渲染帧率: {}FPS", readerRender.GetFPS());
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    label = std::make_shared<UILabelReader>();
    label->AddReader([]() {
        return std::format("物理帧生成时： {:.2f}ms",
                           GET_PhysicsSystem.GetFrameTime());
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    label = std::make_shared<UILabelReader>();
    label->AddReader([]() {
        return std::format("物理帧率: {}FPS", GET_PhysicsSystem.GetFPS());
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    label = std::make_shared<UILabelReader>();
    label->AddReader([]() {
        return std::format("刚体数量: {}", GET_PhysicsSystem.GetBodyCount());
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    label = std::make_shared<UILabelReader>();
    label->AddReader([]() {
        auto &camera = GET_RenderSystem.GetCamera();
        return std::format("相机位置:{}\n缩放:{}", camera.GetPosition(),
                           camera.getZoom());
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 5});

    label = std::make_shared<UILabelReader>();
    label->AddReader([]() {
        auto c = GET_PhysicsSystem.GetRootNode()->GetChildren();
        if (GET_PhysicsSystem.selectIndex < c.size()) {
            auto obj = static_cast<RigidBody *>(
                c[GET_PhysicsSystem.selectIndex].get());
            return std::format("序号: {}, 静止: {}\n位置: {:.4f}, 速度: "
                               "{:.4f}\n旋转: {:.4f}, 角速度: {:.4f}",
                               GET_PhysicsSystem.selectIndex,
                               obj->GetIsStatic(), obj->GetPosition(),
                               obj->GetVelocity(), obj->GetRotationDegrees(),
                               obj->GetAngularVelocityDegrees());
        }
        return std::format("序号: {}", GET_PhysicsSystem.selectIndex);
    });
    panel->AddNewContent(std::move(label), TextAlign::START, TextAlign::START,
                         true, {5, 25});

    GET_EventSystem.AddEventListener(
        SDL_EVENT_KEY_DOWN, [panel](SDL_Event &event) {
            if (event.key.key != SDLK_I)
                return;
            if (!panel->GetEnabled()) {
                panel->SetEnabled(true);
                panel->SetRelativePos({0, panel->GetBarHeight()});
            }
        });

    GET_UIMgr.AddUIComponent(panel);
}

void UpdateApp() {
    if (KeyDown(SDL_SCANCODE_ESCAPE)) {
        SDL_Event quitEvent;
        quitEvent.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quitEvent);
    }
}

void InitPhy() {
    Material m{1, 0.7, 0.5, 0.5};

    auto ground = std::make_shared<BoxBody>(m, glm::vec2(20, 0.2));
    ground->MoveTo({0, -8});
    ground->SetFColorBoundry({0, 0, 0, 1});
    ground->SetColor({77, 120, 204, 255});
    ground->SetIsStatic(true);
    ground->SetName("ground");
    GET_PhysicsSystem.AddObject(ground);

    auto wallL = std::make_shared<BoxBody>(m, glm::vec2(0.2, 10));
    wallL->MoveTo({-4, -4});
    wallL->SetFColorBoundry({0, 0, 0, 1});
    wallL->SetColor({77, 120, 204, 255});
    wallL->SetIsStatic(true);
    wallL->SetName("wall1");
    GET_PhysicsSystem.AddObject(wallL);

    auto wallR = std::make_shared<BoxBody>(m, glm::vec2(0.2, 10));
    wallR->MoveTo({4, -4});
    wallR->SetFColorBoundry({0, 0, 0, 1});
    wallR->SetColor({77, 120, 204, 255});
    wallR->SetIsStatic(true);
    wallR->SetName("wall2");
    GET_PhysicsSystem.AddObject(wallR);

    // here is test for rope
    // auto ObjA = std::make_shared<CircleBody>(m, 0.5);
    // ObjA->MoveTo({0, -6});
    // ObjA->SetName("ObjA");
    // ObjA->SetFColor(RandomFColor());
    // ObjA->SetIsStatic(false);
    // GET_PhysicsSystem.AddObject(ObjA);

    // auto ObjB = std::make_shared<CircleBody>(m, 0.5);
    // ObjB->MoveTo({0, -9});
    // ObjB->SetName("ObjB");
    // ObjB->SetFColor(RandomFColor());
    // ObjB->SetIsStatic(false);
    // GET_PhysicsSystem.AddObject(ObjB);

    // auto constraint = std::make_shared<RopeConstraint>(ObjA.get(),
    // ObjB.get(), ZeroVec2, ZeroVec2, 10);
    // GET_PhysicsSystem.AddConstraint(constraint);

    // here is test for link
    auto ObjA = std::make_shared<CircleBody>(m, 0.5);
    ObjA->MoveTo({-2, 2});
    ObjA->SetName("ObjA");
    ObjA->SetFColor(RandomFColor());
    ObjA->SetIsStatic(false);
    GET_PhysicsSystem.AddObject(ObjA);

    auto ObjB = std::make_shared<CircleBody>(m, 0.5);
    ObjB->MoveTo({0, 8});
    ObjB->SetName("ObjB");
    ObjB->SetFColor(RandomFColor());
    ObjB->SetIsStatic(false);
    GET_PhysicsSystem.AddObject(ObjB);

    auto constraint = std::make_shared<LinkConstraint>(ObjA.get(), ObjB.get(),
                                                       glm::vec2{0.3,0}, glm::vec2{0.3,0}, 1.5);
    GET_PhysicsSystem.AddConstraint(constraint);

    auto constraint1 = std::make_shared<LinkConstraint>(ObjA.get(), ObjB.get(),
    glm::vec2{-0.3,0}, glm::vec2{-0.3,0}, 1.5);
GET_PhysicsSystem.AddConstraint(constraint1);
    // here is test for spring
    // auto ObjA = std::make_shared<CircleBody>(m, 0.5);
    // ObjA->MoveTo({0, -6});
    // ObjA->SetName("ObjA");
    // ObjA->SetFColor(RandomFColor());
    // ObjA->SetIsStatic(false);
    // GET_PhysicsSystem.AddObject(ObjA);

    // auto ObjB = std::make_shared<CircleBody>(m, 0.5);
    // ObjB->MoveTo({0, -9});
    // ObjB->SetName("ObjB");
    // ObjB->SetFColor(RandomFColor());
    // ObjB->SetIsStatic(false);
    // GET_PhysicsSystem.AddObject(ObjB);

    // auto constraint = std::make_shared<SpringConstraint>(ObjA.get(),
    // ObjB.get(), ZeroVec2, ZeroVec2, 10, 5);
    // GET_PhysicsSystem.AddConstraint(constraint);

    // here is test for cord
    // auto ObjA = std::make_shared<CircleBody>(m, 0.5);
    // ObjA->MoveTo({0, -6});
    // ObjA->SetName("ObjA");
    // ObjA->SetFColor(RandomFColor());
    // ObjA->SetIsStatic(false);
    // GET_PhysicsSystem.AddObject(ObjA);

    // auto ObjB = std::make_shared<CircleBody>(m, 0.5);
    // ObjB->MoveTo({0, -9});
    // ObjB->SetName("ObjB");
    // ObjB->SetFColor(RandomFColor());
    // ObjB->SetIsStatic(false);
    // GET_PhysicsSystem.AddObject(ObjB);

    // auto constraint = std::make_shared<CordConstraint>(
    //     ObjA.get(), ObjB.get(), ZeroVec2, ZeroVec2, 10, 5);
    // GET_PhysicsSystem.AddConstraint(constraint);
}

void UpdatePhy() {
    auto children = GET_PhysicsSystem.GetRootNode()->GetChildren();

    Material m{1, 0.7, 0.5, 0.5};
    static int counter = 0;
    if (PKeyDown(SDL_SCANCODE_Q)) {
        auto obj = std::make_shared<BoxBody>(m, RandomPos({0.5, 0.5}, {1, 1}));
        obj->SetFColor(RandomFColor());
        obj->MoveTo(SCREEN2WORLD(MousePos));
        obj->SetName(std::format("the {}'th object", counter++));
        obj->SetIsStatic(false);
        GET_PhysicsSystem.AddObject(obj);
        GET_PhysicsSystem.selectIndex = children.size();
    }

    if (PKeyDown(SDL_SCANCODE_E)) {
        auto obj = std::make_shared<CircleBody>(m, RandomFloat(0.25, 0.5));
        obj->SetFColor(RandomFColor());
        obj->MoveTo(SCREEN2WORLD(MousePos));
        obj->SetName(std::format("the {}'th object", counter++));
        obj->SetIsStatic(false);
        GET_PhysicsSystem.AddObject(obj);
    }

    if (PKeyDown(SDL_SCANCODE_RIGHT)) {
        GET_PhysicsSystem.selectIndex++;
        if (GET_PhysicsSystem.selectIndex >= children.size()) {
            GET_PhysicsSystem.selectIndex = children.size() - 1;
        }
    }
    if (PKeyDown(SDL_SCANCODE_LEFT)) {
        GET_PhysicsSystem.selectIndex--;
        if (GET_PhysicsSystem.selectIndex < 0) {
            GET_PhysicsSystem.selectIndex = 0;
        }
    }
}