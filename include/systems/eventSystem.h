# pragma once

#include <SDL_events.h>
#include <functional>
#include <unordered_map>

using EventCallback = std::function<void(SDL_Event&)>;
using EventHandlerID = size_t;

class EventSystem {
  public:
    static EventSystem& Instance();
    void Init();
    void HandleEvent();
    template<typename T>
    EventHandlerID AddEventListener(SDL_EventType type, T&& callback);

    template<class C, typename Method>
    EventHandlerID AddEventListener(SDL_EventType type, C* instance, Method callback);

    bool RemoveEventListener(SDL_EventType type, EventHandlerID id);
    
  private:
    void EventDispatcher(SDL_Event& event);
    std::unordered_map<SDL_EventType, std::vector<EventCallback>> eventMap_;
};


template <typename T>
EventHandlerID EventSystem::AddEventListener(SDL_EventType type, T &&callback) {
    eventMap_[type].emplace_back(std::forward<T>(callback));
    return reinterpret_cast<EventHandlerID>(&eventMap_[type].back());
}

template <class C, typename Method>
EventHandlerID EventSystem::AddEventListener(SDL_EventType type, C *instance,
                                             Method callback) {
    eventMap_[type].emplace_back([instance, callback](SDL_Event &event) {
        (instance->*callback)(event);
    });
    return reinterpret_cast<EventHandlerID>(&eventMap_[type].back());
}

# define GET_EventSystem EventSystem::Instance()