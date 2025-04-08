# pragma once

#include "SDL3/SDL_pixels.h"
#include "conversion.h"
#include "object.h"
#include <glm/ext/vector_float2.hpp>

class ObjectWorld : public Object {
  protected:
    glm::vec2 position_;
    SDL_FColor color;
  public:
    ObjectWorld() : position_(0, 0) {;};
    ObjectWorld(glm::vec2 position) : position_(position) {;};
    glm::vec2 GetPosition() const { return position_; }
    void SetPosition(const glm::vec2 &pos) { position_ = pos; }
    void SetPosition(float x, float y) { position_ = {x, y}; }
    void SetFColor(SDL_FColor color) {this->color = color;}
    void SetColor(SDL_Color color) {this->color = ToFColor(color);}
    SDL_FColor GetFColor() const {return color;}


    void Init() override{;}
    void Render() override{;}
    void Update(float dt) override{;}
    void PhysicsUpdate(float dt) override{;}
    void HandleEvent(SDL_Event &event) override{;}
    void Destroy() override{;}

    void OnMouseDown(SDL_Event &event) override{;}
    void OnMouseMove(SDL_Event &event) override{;}
    void OnMouseUp(SDL_Event &event) override{;}

};