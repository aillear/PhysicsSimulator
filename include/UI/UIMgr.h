# pragma once   

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include "UIComponent.h"
#include "eventSystem.h"
#include <memory>

/**
 * @brief Manager for all UIComponents.
 *   It will update and render all UIComponents.
 *   workflow: create UIComponent -> add to UIMgr -> update and render in UIMgr -> remove from UIMgr
 *             when a new UIComponent is added, it will be added to the list in the next frame.
 *             when a UIComponent is removed, it will be removed from the list in the next frame.
 *             in ench frame, UIMgr will first init the UIComponents added by last frame
 *             then remove UIComponent that mark to be removed by last frame.
 *             then update all UIComponents in the list.
 *   all the live cyle method will be called recursively.
 */
class UIMgr {
  public:
    static UIMgr& Instance();
    void Init();
    void Destroy();
    void Update(float dt);

    /*
        add/remove new UIComponent to the list, it will be added in the next frame
        if target is nullptr, it will be added to the root
        if target is not nullptr, it will be added to the target's children
        also support add/remove by ID or name
        if targetID or targetName is not found, it will be added to the root
        those functions will recursively search all children
        all the UIComponents need to add to the list so that they can be updated and rendered
    */
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
    // call uiComponents's HandleEvent function recursively
    void HandleSDLEvents(SDL_Event& event);

    std::vector<std::shared_ptr<UIComponent>> uiComponents;
    std::vector<std::shared_ptr<UIComponent>> uiComponentsToAdd;
    std::vector<std::shared_ptr<UIComponent>> uiComponentsToRemove;
    EventHandlerID eventHandler_1, eventHandler_2, eventHandler_3;
};

# define GET_UIMgr UIMgr::Instance()