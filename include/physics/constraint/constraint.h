#pragma once

#include "rigidbody.h"
class Constraint {
  public:
    Constraint(RigidBody *a, RigidBody *b, glm::vec2 localA, glm::vec2 localB,
               float len)
        : bodyA(a), bodyB(b), anchorA(localA), anchorB(localB), maxLength(len) {}
    RigidBody *bodyA;
    RigidBody *bodyB;
    glm::vec2 anchorA;
    glm::vec2 anchorB;
    float maxLength;
    
    virtual void Solve(float dt) = 0;
    virtual void Render() = 0;

  protected:
    bool firstApply = true;
    bool isTight = false;
    glm::vec2 worldA;
    glm::vec2 worldB;
};