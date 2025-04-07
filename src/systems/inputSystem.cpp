#include "inputSystem.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "eventSystem.h"
#include <algorithm>
#include <mutex>
#include <shared_mutex>

InputSystem &InputSystem::Instance() {
    static InputSystem instance;
    return instance;
}

void InputSystem::Init() {
    eventID1_ = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION, [this](SDL_Event &event) {
            mousePos = {event.motion.x, event.motion.y};
        });
}

void InputSystem::Destroy() { keyEventMap_.clear(); }

KeyCallBackHandlerId InputSystem::AddKeyListener(SDL_Keymod mod,
                                                 SDL_Keycode code,
                                                 EventCallback callBack) {
    // write lock
    std::unique_lock lock(mutex_);
    KeyCallBackHandlerId id = idCounter_++;
    keyEventMap_[mod][code].emplace_back(KeyCallBackWrapper{id, callBack});
    return id;
}

bool InputSystem::RemoveKeyListener(SDL_Keymod mod, SDL_Keycode code,
                                    KeyCallBackHandlerId id) {
    std::unique_lock lock(mutex_);
    auto it = keyEventMap_.find(mod);
    if (it == keyEventMap_.end())
        return false;

    auto it_ = it->second.find(code);
    if (it_ == it->second.end())
        return false;

    auto &handlers = it_->second;
    handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
                                  [id](const KeyCallBackWrapper &callBack) {
                                      return callBack.id == id;
                                  }),
                   handlers.end());
    return true;
}

void InputSystem::KeyEventDispatcher(SDL_Event& event) {
    std::shared_lock lock(mutex_);
    auto& delegate = keyEventMap_[event.key.mod][event.key.key];
    for (auto& handler : delegate) {
        handler.callBack(event);
    }
}