#include "UIComponent.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_pixels.h"
#include "renderSystem.h"
#include <glm/ext/vector_float2.hpp>

UIComponent::UIComponent(glm::vec2 leftTop, glm::vec2 widthHeight,
    SDL_FColor color) : leftTopPos(leftTop), widthHeight(widthHeight), color(color) {

    AddUpdateCallBack([this](float){
        if (!this->needToImplementAlignment) return;
        needToImplementAlignment = false;
        ImplementAlignment();
    } );
    
}

bool UIComponent::HitTest(glm::vec2 MousePos) {
    auto pos = GetScreenPos();
    return (MousePos.x >= pos.x && MousePos.x <= pos.x + widthHeight.x &&
            MousePos.y >= pos.y && MousePos.y <= pos.y + widthHeight.y);
}

glm::vec2 UIComponent::GetScreenPos() const {
    if (parent == nullptr || !relateToParent) return leftTopPos;
    return static_cast<UIComponent*>(parent)->GetScreenPos() + leftTopPos;
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

glm::vec2 UIComponent::GetParentRelativePos() const {
    if (parent == nullptr) return {0, 0};
    return static_cast<UIComponent*>(parent)->GetRelativePos();
}

glm::vec2 UIComponent::GetParentScreenPos() const {
    if (parent == nullptr) return {0, 0};
    return static_cast<UIComponent*>(parent)->GetScreenPos();
}

glm::vec2 UIComponent::GetParentWidthHeight() const {
    if (parent == nullptr) return GET_RenderSystem.GetWindowSize();
    return static_cast<UIComponent*>(parent)->GetWidthHeight();
}

void UIComponent::ImplementAlignment() {
    auto parentWH = GetParentWidthHeight();
    switch (xAlign) {
        case TextAlign::START:
            leftTopPos.x = margin.x + offset.x;
            break;
        case TextAlign::CENTER:
            leftTopPos.x = (parentWH.x - widthHeight.x) * 0.5f + offset.x;
            break;
        case TextAlign::END:
            leftTopPos.x = parentWH.x - widthHeight.x - margin.x + offset.x;
            break;
        default:
            break;
    }

    switch (yAlign) {
        case TextAlign::START:
            leftTopPos.y = margin.y + offset.y;
            break;
        case TextAlign::CENTER:
            leftTopPos.y = (parentWH.y - widthHeight.y) * 0.5f + offset.y;
            break;
        case TextAlign::END:
            leftTopPos.y = parentWH.y - widthHeight.y - margin.y + offset.y;
            break;
        default:
            break;
    }
}

void UIComponent::SetMargin(glm::vec2 margin) {
    this->margin = margin;
    needToImplementAlignment = true;
}

void UIComponent::SetOffset(glm::vec2 offset) {
    this->offset = offset;
    needToImplementAlignment = true;
}

void UIComponent::SetAlignModeX(TextAlign xAlign) {
    this->xAlign = xAlign;
    needToImplementAlignment = true;
}

void UIComponent::SetAlignModeY(TextAlign yAlign) {
    this->yAlign = yAlign;
    needToImplementAlignment = true;
}

void UIComponent::SetMarginX(float marginX) {
    this->margin.x = marginX;
    needToImplementAlignment = true;
}

void UIComponent::SetMarginY(float marginY) {
    this->margin.y = marginY;
    needToImplementAlignment = true;
}

void UIComponent::SetOffsetX(float offsetX) {
    this->offset.x = offsetX;
    needToImplementAlignment = true;
}

void UIComponent::SetOffsetY(float offsetY) {
    this->offset.y = offsetY;
    needToImplementAlignment = true;
}

void UIComponent::SetAlignMent(TextAlign xAlign, TextAlign yAlign,
                                glm::vec2 offset, glm::vec2 margin) {
    this->xAlign = xAlign;
    this->yAlign = yAlign;
    this->offset = offset;
    this->margin = margin;
    needToImplementAlignment = true;
}