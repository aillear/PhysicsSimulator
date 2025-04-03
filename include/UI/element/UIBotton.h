#pragma once
#include "SDL3/SDL_stdinc.h"
#include "UIComponent.h"
#include <functional>

using ClickCallback = std::function<void()>;
class UIButton : public UIComponent {
  public:
    UIButton(glm::vec2 leftTop = {0, 0}, glm::vec2 rightBottom = {0, 0},
             SDL_FColor color = {0, 0, 0, 0},
             SDL_FColor colorHover = {0, 0, 0, 0},
             SDL_FColor colorPressed = {0, 0, 0, 0})
        : UIComponent(leftTop, rightBottom, color), colorHover(colorHover),
          colorPressed(colorPressed) {
        ;
    }
    ~UIButton() override = default;
    void Render() override;
    void Update(float dt) override;
    void HandleEvent(SDL_Event &event) override;

    void SetCallBack(ClickCallback call) { callback = std::move(call); }
    void SetColorHover(SDL_FColor color) { colorHover = color; }
    void SetColorPressed(SDL_FColor color) { colorPressed = color; }
    SDL_FColor GetColorHover() const { return colorHover; }
    SDL_FColor GetColorPressed() const { return colorPressed; }

    enum class ButtonState : Uint8 {
        NORMAL = 0,
        HOVER,
        PRESSED,
    };
  private:
    SDL_FColor colorHover;
    SDL_FColor colorPressed;
    ClickCallback callback = nullptr;
    ButtonState state = ButtonState::NORMAL;
};
