#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <memory>
#include <vector>

class UIComponent {
  public:
    virtual ~UIComponent() = default;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleEvent(SDL_Event &event) = 0;
    bool enabled;
    bool zIndex;
    SDL_Color color;
    SDL_Rect rect;
    std::vector<std::shared_ptr<UIComponent>> children;
};