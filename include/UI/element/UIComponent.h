#pragma once

#include "object.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <memory>
#include <vector>

class UIComponent : public Object {
  public:
    bool GetEnabled() const { return enabled; }
    void SetEnabled(bool enabled) { this->enabled = enabled; }
    bool GetZIndex() const { return zIndex; }
    void SetZIndex(bool zIndex) { this->zIndex = zIndex; }
    SDL_Color GetColor() const { return color; }
    void SetColor(SDL_Color color) { this->color = color; }
    void AddChild(std::shared_ptr<UIComponent> child) {
        children.emplace_back(child);
    }
    void RemoveChild(std::shared_ptr<UIComponent> child) {
        children.erase(std::remove(children.begin(), children.end(), child),
                       children.end());
    }
    void ClearChildren() { children.clear(); }
    std::vector<std::shared_ptr<UIComponent>> GetChildren() const {
        return children;
    }

  protected:
    bool enabled;
    bool zIndex;
    SDL_Color color;
    std::vector<std::shared_ptr<UIComponent>> children;
};