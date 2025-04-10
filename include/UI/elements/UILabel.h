#pragma once

#include "UIComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>
#include <glm/ext/vector_float2.hpp>
#include <memory>

class UILabel : public UIComponent {
  public:
    

    UILabel(glm::vec2 leftTop = {0, 0}, SDL_FColor color = {0, 0, 0, 0});
    void Init() override;
    void Render() override;
    void Update(float dt) override;
    void ChangeText(const std::string &text, size_t length = 0);

  private:
    std::shared_ptr<TTF_Text> textObj;
    size_t textLength;
};