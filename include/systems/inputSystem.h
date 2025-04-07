# pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "eventSystem.h"
#include <atomic>
#include <cstddef>
#include <glm/ext/vector_float2.hpp>
#include <list>
#include <shared_mutex>
#include <unordered_map>

using KeyCallBackHandlerId = size_t;
struct KeyCallBackWrapper {
    KeyCallBackHandlerId id;
    EventCallback callBack;
};
/**
 * @brief rely on eventSystem.
 * 
 */
class InputSystem {
  public:
    static InputSystem& Instance();

    void Init();
    void Destroy();
    const glm::vec2& GetMousePos() const { return mousePos; }
    KeyCallBackHandlerId AddKeyListener(SDL_Keymod mod, SDL_Keycode code, EventCallback callBack);
    bool RemoveKeyListener(SDL_Keymod mod, SDL_Keycode code, KeyCallBackHandlerId id);


  private:
    void KeyEventDispatcher(SDL_Event& event);

    InputSystem() = default;
    ~InputSystem() = default;
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;

    std::shared_mutex mutex_;
    std::atomic<KeyCallBackHandlerId> idCounter_{0};
    glm::vec2 mousePos{0, 0};
    EventHandlerID eventID1_;
    EventHandlerID eventID2_;

    std::unordered_map<SDL_Keymod, std::unordered_map<SDL_Keycode, std::list<KeyCallBackWrapper>>> keyEventMap_;
};


# define GET_InputSystem InputSystem::Instance()
# define MousePos GET_InputSystem.GetMousePos()