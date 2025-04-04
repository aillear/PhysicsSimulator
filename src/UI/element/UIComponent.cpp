#include "UIComponent.h"
#include "SDL3/SDL_events.h"
#include <glm/ext/vector_float2.hpp>

bool UIComponent::HitTest(glm::vec2 MousePos) {
    auto pos = GetScreenPos();
    return (MousePos.x >= pos.x && MousePos.x <= pos.x + widthHeight.x &&
            MousePos.y >= pos.y && MousePos.y <= pos.y + widthHeight.y);
}

glm::vec2 UIComponent::GetScreenPos() const {
    if (parent != nullptr) {
        return static_cast<UIComponent*>(parent)->GetScreenPos() + leftTopPos;
    }
    return leftTopPos;
}

void UIComponent::HandleEvent(SDL_Event& event) {
    // current only handle mouse event
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            OnMouseMove(event);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            OnMouseDown(event);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            OnMouseUp(event);
            break;
        }
    }
}