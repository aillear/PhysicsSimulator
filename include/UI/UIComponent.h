#pragma once

#include <memory>
#include <vector>
#include "object.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>

class UIComponent : public Object {
  public:
    virtual ~UIComponent() = default;
    bool enabled;
    bool zIndex;
    SDL_Color color;
    SDL_Rect rect;
    std::vector<std::shared_ptr<UIComponent>> children;
};