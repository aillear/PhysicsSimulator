#include "eventSystem.h"
#include <SDL_events.h>

EventSystem &EventSystem::Instance() {
    static EventSystem instance;
    return instance;
}

bool EventSystem::RemoveEventListener(SDL_EventType type, EventHandlerID id) {
    auto it = eventMap_.find(type);
    if (it == eventMap_.end())
        return false;

    auto &handlers = it->second;
    handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
        [id](const EventCallback &callback) {
                    return reinterpret_cast<EventHandlerID>(&callback) == id;}),
        handlers.end());
    return true;
}

void EventSystem::EventDispatcher(SDL_Event &event) {
    auto& delegate = eventMap_[static_cast<SDL_EventType>(event.type)];
    for (auto& handler : delegate) {
        handler(event);
    }
}

void EventSystem::HandleEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        EventDispatcher(event);
    }
}

void EventSystem::Init() {
    ;
}