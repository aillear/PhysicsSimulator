#pragma once

#include "material.h"
#include "objectWorld.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <memory>

// TODO: add more physics properties
class RigidBody : public ObjectWorld {

  private:
  public:
    RigidBody(glm::vec2 position, float area, Material mate, glm::vec2 wh,
              float radius, bool isStatic, PhysicsShapeType type)
        : ObjectWorld(position), area(area), material(mate),
          mass(area * mate.density), widthHeight(wh), radius(radius),
          isStatic(isStatic), velocity({0, 0}), rotation(0), angularVelocity(0),
          type(type) {
        ;
    }

    void Render() override;
    void PhysicsUpdate(float dt) override;

    void Move(glm::vec2 ds) {position_ += ds;}
    void MoveTo(glm::vec2 destinaion) {position_ = destinaion;}

    bool isStatic;
    float mass;
    float area;
    Material material;
    float radius;
    glm::vec2 widthHeight;
    PhysicsShapeType type;

  protected:
    float rotation;
    float angularVelocity;
    glm::vec2 velocity;
};

std::shared_ptr<RigidBody> CreateCircleBody(float radius, glm::vec2 position,
                                            Material mate);

std::shared_ptr<RigidBody> CreateBoxBody(glm::vec2 wh, glm::vec2 position,
                                         Material mate);