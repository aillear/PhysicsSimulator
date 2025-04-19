#pragma once

#include "constraint.h"
#include "rigidbody.h"

class LinkConstraint : public Constraint {
  public:
    SDL_FColor color{1, 1, 1, 1};
    SDL_FColor tightColor{1, 0, 0, 1};

    LinkConstraint(RigidBody *a, RigidBody *b, glm::vec2 localA,
                   glm::vec2 localB, float len)
        : Constraint(a, b, localA, localB, len) {}

    void Solve(float dt) override;
    void Render() override;
};
