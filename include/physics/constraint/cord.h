#pragma once

#include "constraint.h"
class CordConstraint : public Constraint {
  public:
    SDL_FColor color{1, 1, 1, 1};
    SDL_FColor tightColor{1, 0, 0, 1};
    CordConstraint(RigidBody *a, RigidBody *b, glm::vec2 localA,
                     glm::vec2 localB, float len, float k)
        : Constraint(a, b, localA, localB, len), k(k) {}
    void Solve(float dt) override;
    void Render() override;

  private:
    float k; // spring constant
};