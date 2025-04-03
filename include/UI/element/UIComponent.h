#pragma once

#include "object.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <glm/ext/vector_float2.hpp>

class UIComponent : public Object {
  public:
    // UIComponent does not have a fixed update function, so I delete it
    void fixedUpdate(float dt) = delete;
    void Render() override = 0;
    void Update(float dt) override = 0;
    void HandleEvent(SDL_Event& event) override = 0;
    void Init() override { ; }
    void Destroy() override { ; }
    

    bool GetZIndex() const { return zIndex; }
    void SetZIndex(bool zIndex) { this->zIndex = zIndex; }

    SDL_FColor GetColor() const { return color; }
    void SetColor(SDL_FColor color) { this->color = color; }

    glm::vec2 GetRelativePos() const { return leftTopPos; }
    glm::vec2 GetWorldPos() const;

    virtual bool HitTest(glm::vec2 MousePos);

    UIComponent(glm::vec2 leftTop = {0, 0}, glm::vec2 widthHeight = {0, 0},
                SDL_FColor color = {0, 0, 0, 0})
        : leftTopPos(leftTop), widthHeight(widthHeight), color(color) {}

  protected:
    bool zIndex;
    // this rect is screen pos!!!
    glm::vec2 leftTopPos;
    glm::vec2 widthHeight;
    SDL_FColor color;
    
};