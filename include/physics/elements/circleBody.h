#pragma once

#include "material.h"
#include "rigidbody.h"
#include <glm/ext/vector_float2.hpp>
class CircleBody : public RigidBody {
  public:
    CircleBody(Material mate, glm::vec2 center, float radius);
    float GetRadius() const override {return radius_;}
    void SetRadius(float radius) override {radius_ = radius;}
    
  protected:
    void Render() override;
    void PhysicsUpdate(float dt) override;
    
    float radius_;
};