# pragma once   

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include "UIComponent.h"
#include "eventSystem.h"
class UIMgr {
  public:
    static UIMgr& Instance();
    void Init();
    void Destroy();
    void Update(float dt);

    // add/remove new UIComponent to the list, it will be added in the next frame
    // if target is nullptr, it will be added to the root
    // if target is not nullptr, it will be added to the target's children
    // also support add/remove by ID or name
    // if targetID or targetName is not found, it will be added to the root
    // those functions will recursively search all children
    void AddUIComponent(std::shared_ptr<UIComponent> component, std::shared_ptr<UIComponent> target = nullptr);
    void AddUIComponent(std::shared_ptr<UIComponent> component, Uint32 targetID);
    void AddUIComponent(std::shared_ptr<UIComponent> component, std::string targetName);
    // if not found, do nothing
    void RemoveUIComponent(std::shared_ptr<UIComponent> component);
    void RemoveUIComponent(Uint32 componentID);
    void RemoveUIComponent(std::string componentName);
  
  private:
    UIMgr() = default;
    ~UIMgr() = default;
    UIMgr(const UIMgr&) = delete;
    UIMgr& operator=(const UIMgr&) = delete;

    void HandleSDLEvents(SDL_Event& event);

    std::vector<std::shared_ptr<UIComponent>> uiComponents;
    std::vector<std::shared_ptr<UIComponent>> uiComponentsToAdd;
    std::vector<std::shared_ptr<UIComponent>> uiComponentsToRemove;
    EventHandlerID eventHandler_1, eventHandler_2, eventHandler_3;
};