#pragma once

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
    static InputSystem &Instance();

    void Init();
    void Destroy();

    void Update();
    const glm::vec2 &GetMousePos() const { return mousePos; }

    bool CheckKeyPress(SDL_Scancode key) { return KeyStates[key]; }
    bool CheckKeyDown(SDL_Scancode key);
    bool CheckKeyUp(SDL_Scancode key);

    bool PCheckKeyPress(SDL_Scancode key) { return KeyStates[key]; }
    bool PCheckKeyDown(SDL_Scancode key);
    bool PCheckKeyUp(SDL_Scancode key);

  private:
    InputSystem() = default;
    ~InputSystem() = default;
    InputSystem(const InputSystem &) = delete;
    InputSystem &operator=(const InputSystem &) = delete;

    const bool *KeyStates;
    bool *keyStateLast;
    bool *keyStateTemp;
    int keyStateSize;

    bool *PkeyDownRecord;
    bool *PkeyUpRecord;
    SDL_MouseButtonFlags MouseButtonFlags;
    glm::vec2 mousePos{0, 0};
};

#define GET_InputSystem InputSystem::Instance()
#define MousePos GET_InputSystem.GetMousePos()
#define KeyPress(key) GET_InputSystem.CheckKeyPress(key)
#define KeyDown(key) GET_InputSystem.CheckKeyDown(key)
#define KeyUp(key) GET_InputSystem.CheckKeyUp(key)

#define PKeyPress(key) GET_InputSystem.PCheckKeyPress(key)
#define PKeyDown(key) GET_InputSystem.PCheckKeyDown(key)
#define PKeyUp(key) GET_InputSystem.PCheckKeyUp(key)