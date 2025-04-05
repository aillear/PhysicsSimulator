#include "UILabel.h"
#include "renderSystem.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>

UILabel::~UILabel() {
    TTF_DestroyText(textObj);
}

UILabel::UILabel(glm::vec2 leftTop)
    : UIComponent(leftTop, {0, 0}), textLength(0) {
    textObj = GET_RenderSystem.CreateText("default text", 0);

    int w, h;
    TTF_GetTextSize(textObj, &w, &h);
    textSize.x = static_cast<float>(w);
    textSize.y = static_cast<float>(h);
}

void UILabel::ChangeText(const std::string &text, size_t length) {
    TTF_SetTextString(textObj, text.c_str(), length);
    textLength = length;
    int w, h;
    TTF_GetTextSize(textObj, &w, &h);
    textSize.x = static_cast<float>(w);
    textSize.y = static_cast<float>(h);
}

void UILabel::Init() {}

void UILabel::Render() {
    DrawCommand cmd(ShapeType::TEXT, true);
    cmd.GetComplex().color = color;
    cmd.GetComplex().SetTextPos(leftTopPos);
    GET_RenderSystem.AddUIDrawCommand(std::move(cmd));
}

void UILabel::Update(float dt) {;}