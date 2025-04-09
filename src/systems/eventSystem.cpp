#include "eventSystem.h"
#include <SDL3/SDL_events.h>
#include <mutex>


EventSystem &EventSystem::Instance() {
    static EventSystem instance;
    return instance;
}

bool EventSystem::RemoveEventListener(SDL_EventType type, EventHandlerID id) {
    // write lock
    std::unique_lock lock(mutex_);
    auto it = eventMap_.find(type);
    if (it == eventMap_.end())
        return false;

    auto &handlers = it->second;
    handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
        [id](const CallBackWrapper &callback) {
                    return callback.id == id;}),
        handlers.end());
    return true;
}

void EventSystem::EventDispatcher(SDL_Event &event) {
    // read lock
    std::shared_lock lock(mutex_);
    auto& delegate = eventMap_[static_cast<SDL_EventType>(event.type)];
    for (auto& handler : delegate) {
        handler.callback(event);
    }
}

void EventSystem::HandleEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        EventDispatcher(event);
    }
}

bool EventSystem::Init() {
    return true;
}

void EventSystem::Destroy() {
    std::unique_lock lock(mutex_);
    eventMap_.clear();
}