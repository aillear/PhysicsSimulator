#include "UIMgr.h"
#include "SDL3/SDL_events.h"
#include "UIComponent.h"
#include "UIRoot.h"
#include "logger.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

UIMgr &UIMgr::Instance() {
    static UIMgr instance;
    return instance;
}

bool UIMgr::Init() {
    eventHandler_1 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    eventHandler_2 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    eventHandler_3 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_UP,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    rootNode = std::make_shared<UIRoot>();
    rootNode->SetName("rootUINode");
    F_LOG_INFO("UIMgr initialized, event listeners' id: {}, {}, {}",
               eventHandler_1, eventHandler_2, eventHandler_3);
    return true;
}

void UIMgr::Destroy() {
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_MOTION, eventHandler_1);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN,
                                        eventHandler_2);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_UP,
                                        eventHandler_3);
    for (auto &comp : rootNode->GetChildren()) {
        comp->SetToRemove();
    }
    rootNode->CheckChildToRemove();
    LOG_INFO("ALL UIComponest managed by UIMgr is released.");
}

// manage all UIComponent' live cycle, include init, update, and destroy
void UIMgr::Update(float dt) {
    // remove all the marked
    if (hasRemoveCalled) {
        rootNode->CheckChildToRemove();
        hasRemoveCalled = false;
    }

    // add new UIComponents
    for (auto &comp : uiComponentsToAdd) {
        auto parent = comp->GetTempFather();
        if (parent == nullptr) {
            rootNode->AddChild(comp);
        } else
            parent->AddChild(comp);
        comp->InitWrapper();
    }
    uiComponentsToAdd.clear();

    // update all UIComponents
    rootNode->UpdateWrapper(dt);
    // render all UIComponents
    rootNode->RenderWrapper();
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component,
                           std::shared_ptr<UIComponent> target) {
    if (component == nullptr) {
        throw std::runtime_error(
            "UIMgr::AddUIComponent: try to add a nullptr component.");
    }
    component->tempFather = target.get();
    uiComponentsToAdd.emplace_back(component);
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component,
                           UIComponent *target) {
    if (component == nullptr) {
        throw std::runtime_error(
            "UIMgr::AddUIComponent: try to add a nullptr component.");
    }
    component->tempFather = target;
    uiComponentsToAdd.emplace_back(component);
}
// add/remove by ID
void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component,
                           ObjectID targetID) {
    if (component == nullptr) {
        throw std::runtime_error(
            "UIMgr::AddUIComponent: try to add a nullptr component.");
    }
    Object *target = FindComponentByID(targetID).get();
    component->tempFather = target;
    uiComponentsToAdd.emplace_back(component);
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component,
                           std::string targetName) {
    if (component == nullptr) {
        throw std::runtime_error(
            "UIMgr::AddUIComponent: try to add a nullptr component.");
    }
    Object *target = FindComponentByName(targetName).get();
    component->tempFather = target;
    uiComponentsToAdd.emplace_back(component);
}

void UIMgr::AddUIComponent(UIComponent *component,
                           std::shared_ptr<UIComponent> target) {
    AddUIComponent(std::shared_ptr<UIComponent>(component), target);
}
void UIMgr::AddUIComponent(UIComponent *component, UIComponent *target) {
    AddUIComponent(std::shared_ptr<UIComponent>(component), target);
}
void UIMgr::AddUIComponent(UIComponent *component, ObjectID targetID) {
    AddUIComponent(std::shared_ptr<UIComponent>(component), targetID);
}
void UIMgr::AddUIComponent(UIComponent *component, std::string targetName) {
    AddUIComponent(std::shared_ptr<UIComponent>(component), targetName);
}

void UIMgr::RemoveUIComponent(std::shared_ptr<UIComponent> component) {
    component->SetToRemove();
    hasRemoveCalled = true;
}

void UIMgr::RemoveUIComponent(ObjectID componentID) {
    auto removeComponent = FindComponentByID(componentID);
    removeComponent->SetToRemove();
    hasRemoveCalled = true;
}

void UIMgr::RemoveUIComponent(std::string componentName) {
    auto removeComponent = FindComponentByName(componentName);
    removeComponent->SetToRemove();
    hasRemoveCalled = true;
}

void UIMgr::HandleSDLEvents(SDL_Event &event) {
    rootNode->HandleEventWrapper(event);
}

std::shared_ptr<UIComponent> UIMgr::FindComponentByID(ObjectID id) {
    for (auto &comp : uiComponentsToAdd) {
        if (comp->GetID() == id)
            return comp;
    }
    return std::static_pointer_cast<UIComponent>(rootNode->GetChildByID(id));
}

std::shared_ptr<UIComponent> UIMgr::FindComponentByName(std::string name) {
    for (auto &comp : uiComponentsToAdd) {
        if (comp->GetName() == name)
            return comp;
    }
    return std::static_pointer_cast<UIComponent>(
        rootNode->GetChildByName(name));
}