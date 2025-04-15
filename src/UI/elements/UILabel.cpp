#include "UILabel.h"
#include "SDL3/SDL_pixels.h"
#include "renderSystem.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>

UILabel::UILabel(const std::string &text)
    : textLength(0) {
    textObj = GET_RenderSystem.CreateText(text, 0);
    int w, h;
    TTF_GetTextSize(textObj.get(), &w, &h);
    widthHeight.x = static_cast<float>(w);
    widthHeight.y = static_cast<float>(h);
    SetFColor({1,1,1,1});
}

void UILabel::ChangeText(const std::string &text, size_t length) {
    TTF_SetTextString(textObj.get(), text.c_str(), length);
    textLength = length;
    int w, h;
    TTF_GetTextSize(textObj.get(), &w, &h);
    widthHeight.x = static_cast<float>(w);
    widthHeight.y = static_cast<float>(h);
    ImplementAlignment();
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

