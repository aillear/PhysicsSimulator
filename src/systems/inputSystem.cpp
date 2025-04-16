#include "inputSystem.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"
#include <cstddef>
#include <cstring>
#include <utility>



InputSystem &InputSystem::Instance() {
    static InputSystem instance;
    return instance;
}

void InputSystem::Init() {
    KeyStates = SDL_GetKeyboardState(&keyStateSize);
    keyStateLast = new bool[keyStateSize];
    keyStateTemp = new bool[keyStateSize];
    PkeyDownRecord = new bool[keyStateSize];
    PkeyUpRecord = new bool[keyStateSize];
    memcpy(keyStateLast, KeyStates, keyStateSize * sizeof(bool));
    memset(keyStateTemp, 0, keyStateSize * sizeof(bool));
    memset(PkeyDownRecord, 0, keyStateSize * sizeof(bool));
    memset(PkeyUpRecord, 0, keyStateSize * sizeof(bool));
}

void InputSystem::Update() {
    MouseButtonFlags = SDL_GetMouseState(&mousePos.x, &mousePos.y);
    std::swap(keyStateLast, keyStateTemp);
    memcpy(keyStateTemp, KeyStates, keyStateSize * sizeof(bool));
}

bool InputSystem::CheckKeyUp(SDL_Scancode key) {
    return !KeyStates[key] && keyStateLast[key];
}

bool InputSystem::CheckKeyDown(SDL_Scancode key) {
    return KeyStates[key] && !keyStateLast[key];
}

bool InputSystem::PCheckKeyUp(SDL_Scancode key) {
    if (KeyStates[key]) {
        PkeyUpRecord[key] = true;
        return false;
    }

    if (PkeyUpRecord[key]) {
        PkeyUpRecord[key] = false;
        return true;
    }

    return false;
}

bool InputSystem::PCheckKeyDown(SDL_Scancode key) {
    if (!KeyStates[key]) {
        PkeyDownRecord[key] = false;
        return false;
    };

    if (!PkeyDownRecord[key]) {
        PkeyDownRecord[key] = true;
        return true;
    } 
    return false;
}

void InputSystem::Destroy() { 
    delete [] keyStateLast;
    delete [] keyStateTemp;
    delete [] PkeyDownRecord;
    delete [] PkeyUpRecord;
}

