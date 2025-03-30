#pragma once
#include "UIComponent.h"
#include "vector2.h"
#include <glm/ext/vector_int2.hpp>

class UIButton : public UIComponent {
  public:
    UIButton(glm::ivec2 leftTop, glm::ivec2 rightBottom, SDL_Color color,
             SDL_Color colorHover, SDL_Color colorPressed);
    ~UIButton() override = default;
    void render() override;
    void update() override;
    void handleEvent(SDL_Event &event) override;

  private:
    SDL_Color colorHover;
    SDL_Color colorPressed;

    bool isHovering = false;
    bool isPressed = false;
    bool isClicked = false;
};
