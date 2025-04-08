# include "UIMgr.h"
#include "SDL3/SDL_events.h"
#include "UIComponent.h"
#include "logger.h"
#include "object.h"
#include <cstddef>
#include <memory>
#include <string>


UIMgr& UIMgr::Instance() {
    static UIMgr instance;
    return instance;
}

bool UIMgr::Init() {
    eventHandler_1 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION,
        [this](SDL_Event& event) {HandleSDLEvents(event);}
    );

    eventHandler_2 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN,
        [this](SDL_Event& event) {HandleSDLEvents(event);}
    );

    eventHandler_3 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_UP,
        [this](SDL_Event& event) {HandleSDLEvents(event);}
    );

    F_LOG_INFO("UIMgr initialized, event listeners' id: {}, {}, {}", 
        eventHandler_1, eventHandler_2, eventHandler_3);
    return true;
}

void UIMgr::Destroy() {
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_MOTION, eventHandler_1);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN, eventHandler_2);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_UP, eventHandler_3);
}

// manage all UIComponent' live cycle, include init, update, and destroy
void UIMgr::Update(float dt) {
    // remove all the marked
    if (hasRemoveCalled) {
        rootNode->CheckChildToRemove();
        hasRemoveCalled = false;
    }

    // add new UIComponents
    for (auto& comp : uiComponentsToAdd) {
        auto parent = comp->GetParent();
        if (parent == nullptr) {
            rootNode->AddChild(comp);
        }
        else parent->AddChild(comp);
        comp->InitWrapper();
    }
    uiComponentsToAdd.clear();

    // update all UIComponents
    rootNode->UpdateWrapper(dt);
    // render all UIComponents
    rootNode->RenderWrapper();
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component, std::shared_ptr<UIComponent> target) {
    component->SetParent(target.get());
    uiComponentsToAdd.push_back(component);
}

// add/remove by ID
void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component, ObjectID targetID) {
    Object* target = FindComponentByID(targetID).get();
    component->SetParent(target);
    uiComponentsToAdd.push_back(component);
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component, std::string targetName) {
    Object* target = FindComponentByName(targetName).get();
    component->SetParent(target);
    uiComponentsToAdd.push_back(component);
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

void UIMgr::HandleSDLEvents(SDL_Event& event) {
    rootNode->HandleEventWrapper(event);
}

std::shared_ptr<UIComponent> UIMgr::FindComponentByID(ObjectID id) {
    for (auto& comp: uiComponentsToAdd) {
        if (comp->GetID() == id) return comp;
    }
    return std::static_pointer_cast<UIComponent>(rootNode->GetChildByID(id));
}

std::shared_ptr<UIComponent> UIMgr::FindComponentByName(std::string name) {
    for (auto& comp: uiComponentsToAdd) {
        if (comp->GetName() == name) return comp;
    }
    return std::static_pointer_cast<UIComponent>(rootNode->GetChildByName(name));
}