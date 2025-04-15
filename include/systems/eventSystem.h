#pragma once

#include <SDL3/SDL_events.h>
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>


using EventCallback = std::function<void(SDL_Event &)>;
using EventHandlerID = size_t;

struct CallBackWrapper {
    EventHandlerID id;
    EventCallback callback;
};

class EventSystem {
  public:
    static EventSystem &Instance();
    bool Init();
    void Destroy();
    void HandleEvent();
    template <typename T>
    EventHandlerID AddEventListener(Uint32 type, T &&callback);

    template <class C, typename Method>
    EventHandlerID AddEventListener(Uint32 type, C *instance, Method callback);

    void EventDispatcher(SDL_Event &event);
    bool RemoveEventListener(Uint32 type, EventHandlerID id);

  private:
    ~EventSystem() = default;
    std::unordered_map<Uint32, std::list<CallBackWrapper>> eventMap_;
    std::shared_mutex mutex_;
    std::atomic<EventHandlerID> idCounter_{0};
};

template <typename T>
EventHandlerID EventSystem::AddEventListener(Uint32 type, T &&callback) {
    // write lock
    std::unique_lock lock(mutex_);
    EventHandlerID id = idCounter_++;
    eventMap_[type].push_back({id, std::forward<T>(callback)});
    return id;
}

template <class C, typename Method>
EventHandlerID EventSystem::AddEventListener(Uint32 type, C *instance,
                                             Method callback) {
    // write lock
    std::unique_lock lock(mutex_);
    EventHandlerID id = idCounter_++;
    eventMap_[type].push_back({id, [instance, callback](SDL_Event &event) {
                                   (instance->*callback)(event);
                               }});
    return id;
}

#define GET_EventSystem EventSystem::Instance()