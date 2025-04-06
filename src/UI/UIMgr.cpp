# include "UIMgr.h"
#include "SDL3/SDL_events.h"
#include "UIComponent.h"
#include "logger.h"
#include <string>


UIMgr& UIMgr::Instance() {
    static UIMgr instance;
    return instance;
}

void UIMgr::Init() {
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
}

void UIMgr::Destroy() {
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_MOTION, eventHandler_1);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN, eventHandler_2);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_UP, eventHandler_3);
}

// manage all UIComponent' live cycle, include init, update, and destroy
void UIMgr::Update(float dt) {
    // add new UIComponents
    for (auto& comp : uiComponentsToAdd) {
        auto parent = comp->GetParent();
        if (parent == nullptr) {
            uiComponents.emplace_back(comp);
        }
        else parent->AddChild(comp);
        comp->InitWrapper();
    }
    uiComponentsToAdd.clear();

    // remove UIComponents
    for (auto& comp : uiComponentsToRemove) {
        auto parent =  comp->GetParent();
        if (parent == nullptr) {
            auto it = std::remove(uiComponents.begin(), uiComponents.end(), comp);
            if (it != uiComponents.end()) {
                uiComponents.erase(it, uiComponents.end());
            }
        }
        else comp->GetParent()->RemoveChild(comp);
        
        comp->DestroyWrapper();
    }
    uiComponentsToRemove.clear();

    // update all UIComponents
    for (auto& comp : uiComponents) {
        if (comp->GetEnabled()) {
            comp->UpdateWrapper(dt);
        }
    }
    // render all UIComponents
    for(auto& comp : uiComponents) {
        if (comp->GetEnabled()) {
            comp->RenderWrapper();
        }
    }
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component, std::shared_ptr<UIComponent> target) {
    component->SetParent(target.get());
    uiComponentsToAdd.push_back(component);
}

// add/remove by ID, only fond in uiComponents list
void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component, Uint32 targetID) {
    UIComponent* target = nullptr;
    for (auto& comp : uiComponents) {
        target = static_cast<UIComponent*>(comp->GetChildByID(targetID).get());
        if (target != nullptr) {
            break;
        }
    }
    component->SetParent(target);
    uiComponentsToAdd.push_back(component);
}

void UIMgr::AddUIComponent(std::shared_ptr<UIComponent> component, std::string targetName) {
    UIComponent* target = nullptr;
    for (auto& comp : uiComponents) {
        target = static_cast<UIComponent*>(comp->GetChildByName(targetName).get());
        if (target != nullptr) {
            break;
        }
    }
    component->SetParent(target);
    uiComponentsToAdd.push_back(component);
}

void UIMgr::RemoveUIComponent(std::shared_ptr<UIComponent> component) {
    auto id = component->GetID();
    RemoveUIComponent(id);
}

void UIMgr::RemoveUIComponent(Uint32 componentID) {
    auto it = std::remove_if(uiComponents.begin(), uiComponents.end(),
                             [componentID](const std::shared_ptr<UIComponent>& comp) {
                                 return comp->GetID() == componentID;
                             });
    if (it != uiComponents.end()) {
        uiComponentsToRemove.emplace_back(*it);
        uiComponents.erase(it, uiComponents.end());
    }
}

void UIMgr::RemoveUIComponent(std::string componentName) {
    auto it = std::remove_if(uiComponents.begin(), uiComponents.end(),
                             [componentName](const std::shared_ptr<UIComponent>& comp) {
                                 return comp->GetName() == componentName;
                             });
    if (it != uiComponents.end()) {
        uiComponentsToRemove.push_back(*it);
        uiComponents.erase(it, uiComponents.end());
    }
}

void UIMgr::HandleSDLEvents(SDL_Event& event) {
    for (auto it = uiComponents.rbegin(); it != uiComponents.rend(); it++) {
        if (it->get()->GetEnabled()) {
            it->get()->HandleEventWrapper(event);
        }
    }
}