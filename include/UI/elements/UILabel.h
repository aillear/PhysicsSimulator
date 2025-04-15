#pragma once

#include "UIComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>
#include <glm/ext/vector_float2.hpp>
#include <memory>

class UILabel : public UIComponent {
  public:
    

    UILabel(const std::string& text = "label");
    void Init() override;
    void Render() override;
    void Update(float dt) override;
    void ChangeText(const std::string &text, size_t length = 0);

  private:
    std::shared_ptr<TTF_Text> textObj;
    size_t textLength;
};