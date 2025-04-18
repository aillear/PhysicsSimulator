#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include "UIComponent.h"
#include "conversion.h"
#include <functional>

using ClickCallback = std::function<void(SDL_Event &)>;
class UIButton : public UIComponent {
  public:
    enum class ButtonState : Uint8 {
        NORMAL = 0,
        HOVER,
        PRESSED,
    };
    UIButton(glm::vec2 widthHeight = {0, 0}) : UIComponent(widthHeight) { ; }
    ~UIButton() override = default;

    void SetCallBack(ClickCallback call) { callback = std::move(call); }
    void SetColorHover(SDL_Color color) { colorHover = ToFColor(color); }
    void SetColorPressed(SDL_Color color) { colorPressed = ToFColor(color); }
    void SetFColorHover(SDL_FColor color) { colorHover = color; }
    void SetFColorPressed(SDL_FColor color) { colorPressed = color; }
    SDL_FColor GetColorHover() const { return colorHover; }
    SDL_FColor GetColorPressed() const { return colorPressed; }

  protected:
    void Render() override;
    void Update(float dt) override;
    void OnMouseMove(SDL_Event &event) override;
    void OnMouseDown(SDL_Event &event) override;
    void OnMouseUp(SDL_Event &event) override;

  private:
    SDL_FColor colorHover;
    SDL_FColor colorPressed;
    ClickCallback callback = nullptr;
    ButtonState state = ButtonState::NORMAL;
};
