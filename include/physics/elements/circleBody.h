#pragma once

#include "material.h"
#include "renderSystem.h"
#include "rigidbody.h"
#include <glm/ext/vector_float2.hpp>
class CircleBody : public RigidBody {
  public:
    CircleBody(Material mate, float radius);
    float GetRadius() const override {return radius_;}
    void SetRadius(float radius) override {radius_ = radius;}

    const GlmCircle GetCircle() const override;

  protected:
    void Render() override;
    void PhysicsUpdate(float dt) override;
    void GetAABBUpdated() override;
    void CalRotateIntertia() override;


    float radius_;
};