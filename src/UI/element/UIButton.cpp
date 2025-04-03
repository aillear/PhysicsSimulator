# include "UIBotton.h"
# include "renderSystem.h"

void UIButton::Render() {
    // Check if the button is enabled before rendering
    if (!enabled) return;

    DrawCommand cmd(ShapeType::RECT, true);
    glm::vec2 p1 = GetWorldPos();
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

