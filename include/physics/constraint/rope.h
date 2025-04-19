// rope_constraint.h
#pragma once
#include "constraint.h"

class RopeConstraint : public Constraint {
  public:
    SDL_FColor color{1, 1, 1, 1};
    SDL_FColor tightColor{1, 0, 0, 1};

    RopeConstraint(RigidBody *a, RigidBody *b, glm::vec2 localA,
                   glm::vec2 localB, float len)
        : Constraint(a, b, localA, localB, len) {}

    void Solve(float dt) override;
    void Render() override;
};
