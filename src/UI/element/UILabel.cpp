#include "UILabel.h"
#include "SDL3/SDL_pixels.h"
#include "renderSystem.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>

UILabel::UILabel(glm::vec2 leftTop, SDL_FColor color)
    : UIComponent(leftTop, {0, 0}, color), textLength(0) {
    textObj = GET_RenderSystem.CreateText("default text", 0);
    int w, h;
    TTF_GetTextSize(textObj.get(), &w, &h);
    widthHeight.x = static_cast<float>(w);
    widthHeight.y = static_cast<float>(h);
}

void UILabel::ChangeText(const std::string &text, size_t length) {
    TTF_SetTextString(textObj.get(), text.c_str(), length);
    textLength = length;
    int w, h;
    TTF_GetTextSize(textObj.get(), &w, &h);
    widthHeight.x = static_cast<float>(w);
    widthHeight.y = static_cast<float>(h);
}

void UILabel::Init() {}

void UILabel::Render() {
    DrawCommand cmd(ShapeType::TEXT, true);
    cmd.GetComplex().color = color;
    cmd.GetComplex().SetTextPos(GetScreenPos());
    cmd.GetComplex().aabb.w = widthHeight.x;
    cmd.GetComplex().aabb.h = widthHeight.y;
    cmd.GetComplex().data = textObj;     // forget to add this!! takes me a long time to find this bug.
    GET_RenderSystem.AddUIDrawCommand(std::move(cmd));
}

void UILabel::Update(float dt) {;}

