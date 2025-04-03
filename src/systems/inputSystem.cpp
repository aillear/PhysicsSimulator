# include "inputSystem.h"

InputSystem& InputSystem::Instance() {
    static InputSystem instance;
    return instance;
}

void InputSystem::Init() {
    eventID_ = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION,
        [this](SDL_Event& event) { mousePos = {event.motion.x, event.motion.y}; }
    );
}