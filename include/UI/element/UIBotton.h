#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include "UIComponent.h"
#include <functional>

using ClickCallback = std::function<void(SDL_Event &)>;
class UIButton : public UIComponent {
  public:
    enum class ButtonState : Uint8 {
        NORMAL = 0,
        HOVER,
        PRESSED,
    };
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

    void SetCallBack(ClickCallback call) { callback = std::move(call); }
    void SetColorHover(SDL_FColor color) { colorHover = color; }
    void SetColorPressed(SDL_FColor color) { colorPressed = color; }
    SDL_FColor GetColorHover() const { return colorHover; }
    SDL_FColor GetColorPressed() const { return colorPressed; }

    void OnMouseMove(SDL_Event &event) override;
    void OnMouseDown(SDL_Event &event) override;
    void OnMouseUp(SDL_Event &event) override;

  private:
    SDL_FColor colorHover;
    SDL_FColor colorPressed;
    ClickCallback callback = nullptr;
    ButtonState state = ButtonState::NORMAL;
};
