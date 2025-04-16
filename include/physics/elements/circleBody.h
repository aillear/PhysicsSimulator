#pragma once

#include "material.h"
#include "renderSystem.h"
#include "rigidbody.h"
#include <glm/ext/vector_float2.hpp>
class CircleBody : public RigidBody {
  public:
    CircleBody(Material mate, glm::vec2 center, float radius);
    float GetRadius() const override {return radius_;}
    void SetRadius(float radius) override {radius_ = radius;}

    const GlmCircle GetCircle() const override;

    void Move(glm::vec2 ds) override;
    void MoveTo(glm::vec2 destinaion) override;

    
  protected:
    void Render() override;
    void PhysicsUpdate(float dt) override;
    void GetAABBUpdated() override;
    void CalRotateIntertia() override;


    float radius_;
};