# pragma once

#include "UIComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>
#include <glm/ext/vector_float2.hpp>

class UILabel : public UIComponent {
  public:
    UILabel(glm::vec2 leftTop = {0, 0});

    void Init() override;
    void Render() override;
    void Update(float dt) override;
    void ChangeText(const std::string &text, size_t length = 0);
  private:
    std::shared_ptr<TTF_Text> textObj;
    glm::vec2 textSize; // text size in pixels, width and color. this is use for other component to get the size of the text.
    size_t textLength;
};