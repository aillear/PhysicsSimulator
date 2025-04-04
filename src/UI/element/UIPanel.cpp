# include "UIPanel.h"
#include "renderSystem.h"
#include <glm/ext/vector_float2.hpp>

void UIPanel::Render() {
    if (!enabled) return;
    auto pos = GetScreenPos();
    DrawCommand cmd(ShapeType::RECT, true);
    cmd.GetBase().rect = {pos, pos + widthHeight};
    cmd.GetBase().color = color;
    GET_RenderSystem.AddUIDrawCommand(std::move(cmd));

    // render the bar
    pos.y -= barHeight;
    cmd = DrawCommand(ShapeType::RECT, true);
    cmd.GetBase().rect = {pos, pos + glm::vec2(widthHeight.x, barHeight)};
    cmd.GetBase().color = barColor;
    GET_RenderSystem.AddUIDrawCommand(std::move(cmd));
}

void UIPanel::Update(float dt) {;}

void UIPanel::OnMouseDown(SDL_Event &event) {
    if (!event.button.down) return;
    if (!HitTest({event.button.x, event.button.y})) return;
    if (event.button.button != SDL_BUTTON_LEFT) return; // only left button
    isDragging = true;
    lastPosition = {event.button.x, event.button.y};
    event.button.down = false; // reset the button state
}

void UIPanel::OnMouseUp(SDL_Event &event) {
    if (event.button.button != SDL_BUTTON_LEFT) return;

    isDragging = false;
}

void UIPanel::OnMouseMove(SDL_Event &event) {
    if (!isDragging) return;

    const glm::vec2 newMousePos = {event.motion.x, event.motion.y};
    leftTopPos += newMousePos - lastPosition;
    lastPosition = newMousePos;
}

bool UIPanel::HitTest(glm::vec2 MousePos) {
    if (MousePos.x < leftTopPos.x || MousePos.x > leftTopPos.x + widthHeight.x) return false;
    if (MousePos.y < leftTopPos.y - barHeight || MousePos.y > leftTopPos.y) return false;
    return true;
}