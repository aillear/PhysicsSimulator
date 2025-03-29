#pragma once
#include "UIComponent.h"
#include "vector2.h"

class UIButton : public UIComponent {
  public:
    UIButton(Vector2i leftTop, Vector2i rightBottom, SDL_Color color,
             SDL_Color colorHover, SDL_Color colorPressed);
    ~UIButton() override = default;
    void draw() override;
    void update() override;
    void handleEvent(SDL_Event &event) override;

  private:
    SDL_Color colorHover;
    SDL_Color colorPressed;

    bool isHovering = false;
    bool isPressed = false;
    bool isClicked = false;
};
