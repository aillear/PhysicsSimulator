# pragma once

#include "eventSystem.h"
#include <glm/ext/vector_float2.hpp>

/**
 * @brief useless? May be it will be useful in the future.
 * 
 */
class InputSystem {
  public:
    static InputSystem& Instance();

    void Init();
    void Destroy();
    const glm::vec2& GetMousePos() const { return mousePos; }


  private:
    InputSystem() = default;
    ~InputSystem() = default;
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;

    glm::vec2 mousePos{0, 0};
    EventHandlerID eventID_;
};


# define GET_InputSystem InputSystem::Instance()
# define MousePos GET_InputSystem.GetMousePos()