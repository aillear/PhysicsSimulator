#include "UIComponent.h"
#include <glm/ext/vector_float2.hpp>

bool UIComponent::HitTest(glm::vec2 MousePos) {
    return (MousePos.x >= leftTopPos.x && MousePos.x <= leftTopPos.x + widthHeight.x &&
            MousePos.y >= leftTopPos.y && MousePos.y <= leftTopPos.y + widthHeight.y);
}

glm::vec2 UIComponent::GetWorldPos() const {
    if (parent != nullptr) {
        return static_cast<UIComponent*>(parent)->GetWorldPos() + leftTopPos;
    }
    return leftTopPos;
}
