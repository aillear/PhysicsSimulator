# pragma once   

#include "vector2.h"
#include <SDL_events.h>
#include <SDL_rect.h>

class UIComponent {
  public:
    virtual ~UIComponent() = default;
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleEvent(SDL_Event& event) = 0;

    bool enabled;
    bool zIndex;
    struct{Vector2f a, b;} rect;
};