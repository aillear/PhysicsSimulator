#include "inputSystem.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"
#include <cstddef>



InputSystem &InputSystem::Instance() {
    static InputSystem instance;
    return instance;
}

void InputSystem::Init() {
    KeyStates = SDL_GetKeyboardState(nullptr);
}

void InputSystem::Update() {
    MouseButtonFlags = SDL_GetMouseState(&mousePos.x, &mousePos.y);
}

void InputSystem::Destroy() { 

}

