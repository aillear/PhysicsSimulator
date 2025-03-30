# pragma once

#include "object.h"
#include <glm/ext/vector_float2.hpp>

class ObjectWorld : public Object {
  protected:
    glm::vec2 position_;
  public:
    ObjectWorld() : position_(0, 0) {;};
    ObjectWorld(glm::vec2 position) : position_(position) {;};
    glm::vec2 getPosition() const { return position_; }
    void setPosition(const glm::vec2 &pos) { position_ = pos; }
    void setPosition(float x, float y) { position_ = {x, y}; }
};