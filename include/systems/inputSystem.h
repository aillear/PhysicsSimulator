# pragma once

#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_scancode.h"
#include <cstddef>
#include <glm/ext/vector_float2.hpp>


/**
 * @brief rely on eventSystem.
 * 
 */
class InputSystem {
  public:
    static InputSystem& Instance();

    void Init();
    void Destroy();

    void Update();
    const glm::vec2& GetMousePos() const { return mousePos; }

    bool CheckKeyDown(SDL_Scancode key) {return KeyStates[key];}


  private:

    InputSystem() = default;
    ~InputSystem() = default;
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;

    const bool* KeyStates;
    SDL_MouseButtonFlags MouseButtonFlags;
    glm::vec2 mousePos{0, 0};
};


# define GET_InputSystem InputSystem::Instance()
# define MousePos GET_InputSystem.GetMousePos()
# define KeyState(key) GET_InputSystem.CheckKeyDown(key)