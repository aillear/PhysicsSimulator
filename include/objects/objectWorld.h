#pragma once

#include "conversion.h"
#include "object.h"
#include <glm/ext/vector_float2.hpp>


class ObjectWorld : public Object {
  public:
    ObjectWorld(glm::vec2 position = {0, 0}) : position_(position) { ; };
    glm::vec2 GetPosition() const { return position_; }
    virtual void SetPosition(const glm::vec2 &pos) { position_ = pos; }
    virtual void SetPosition(float x, float y) { position_ = {x, y}; }
    virtual void SetFColor(SDL_FColor color) { this->color_ = color; }
    virtual void SetColor(SDL_Color color) { this->color_ = ToFColor(color); }
    SDL_FColor GetFColor() const { return color_; }
    SDL_Color GetColor() const {return ToColor(color_);}

    void OnMouseDown(SDL_Event &event) override { ; }
    void OnMouseMove(SDL_Event &event) override { ; }
    void OnMouseUp(SDL_Event &event) override { ; }
    void OnKeyDown(SDL_Event &event) override { ; }
    void OnKeyUp(SDL_Event &event) override { ; }

  protected:
    void Init() override { ; }
    void Render() override = 0;
    void Update(float dt) override { ; }
    void PhysicsUpdate(float dt) override = 0;
    void HandleEvent(SDL_Event &event) override { ; }
    void Destroy() override { ; }

    glm::vec2 position_;
    SDL_FColor color_;
};