#include "SDL3/SDL_events.h"
#include "UIButton.h"
#include "renderSystem.h"

void UIButton::Render() {
    // Check if the button is enabled before rendering
    if (!enabled) return;

    DrawCommand cmd(ShapeType::RECT, true);
    glm::vec2 p1 = GetScreenPos();
    cmd.GetBase().rect = {p1, p1 + widthHeight};
    
    // Set the color based on the button state
    switch (state) {
        case ButtonState::NORMAL:
            cmd.GetBase().color = color;
            break;
        case ButtonState::HOVER:
            cmd.GetBase().color = colorHover;
            break;
        case ButtonState::PRESSED:
            cmd.GetBase().color = colorPressed;
            break;
    }
    GET_RenderSystem.AddUIDrawCommand(std::move(cmd));
}

void UIButton::Update(float dt) {;}

// only when mouse is pressed, then set the state
void UIButton::OnMouseDown(SDL_Event &event) {
    if (!event.button.down) return;
    if (event.button.button != SDL_BUTTON_LEFT) return;
    if (!HitTest({event.button.x, event.button.y})) return;
    callback(event);
    state = ButtonState::PRESSED;
    event.button.down = false; // reset the button state
}

void UIButton::OnMouseUp(SDL_Event &event) {
    if (HitTest({event.button.x, event.button.y})) state = ButtonState::HOVER;
    else state = ButtonState::NORMAL;
}

// I am genius
void UIButton::OnMouseMove(SDL_Event &event) {
    if (state == ButtonState::PRESSED) return; // do not change state when pressed

    bool isIn = HitTest({event.motion.x, event.motion.y});
    if (state == ButtonState::HOVER && (!isIn || event.motion.reserved == 1)) {
        state = ButtonState::NORMAL;
        return;
    }
    // use reserved to ignore hit event
    if (isIn) {
        state = ButtonState::HOVER;
        event.motion.reserved = 1;
    }
}