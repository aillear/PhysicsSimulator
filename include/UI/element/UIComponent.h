#pragma once

#include "object.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <glm/ext/vector_float2.hpp>

/**
 * @brief UI object's base class.
 *    provide basic hit test and get relative and absolute position methods.
 *    the topLeftPos means the position relate to it's parent's top left corner
 *    it's in screen coordinate system.
 *    you can get absolute position in screen by GetWorldPos(), it will
 * recursionly get all the parents' position and add them up.
 */
class UIComponent : public Object {
  public:
    void Render() override = 0;
    void Update(float dt) override = 0;
    void Init() override { ; }
    void Destroy() override { ; }
    // TODO:only handle mouse event? may change in the future.
    void HandleEvent(SDL_Event &event) override;

    bool GetZIndex() const { return zIndex; }
    void SetZIndex(bool zIndex) { this->zIndex = zIndex; }

    SDL_FColor GetColor() const { return color; }
    void SetColor(SDL_FColor color) { this->color = color; }

    glm::vec2 GetRelativePos() const { return leftTopPos; }
    glm::vec2 GetWorldPos() const;

    virtual bool HitTest(glm::vec2 MousePos);

    void PhysicsUpdate(float dt) override {;}

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