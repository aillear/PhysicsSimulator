#pragma once

#include "SDL3/SDL_pixels.h"
#include "conversion.h"
#include "material.h"
#include "objectWorld.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <string>

// TODO: add more physics properties
class RigidBody : public ObjectWorld {
  public:
    RigidBody(glm::vec2 position, Material mate, PhysicsShapeType type)
        : ObjectWorld(position), material(mate), velocity({0, 0}), rotation(0),
          angularVelocity(0), type(type), boundaryColor({1, 1, 1, 1}) {
        ;
    }

    void Move(glm::vec2 ds) { position_ += ds; }
    void MoveTo(glm::vec2 destinaion) { position_ = destinaion; }
    PhysicsShapeType GetType() { return type; }
    float GetMass() const { return mass; }
    float GetArea() const { return area; }
    float GetRotation() const { return rotation; }
    float GetAngularVelocity() const { return angularVelocity; }
    glm::vec2 GetVelocity() const { return velocity; }
    void SetVelocity(glm::vec2 v) { velocity = v; }
    bool GetIsStatic() { return isStatic; }
    void SetIsStatic(bool value) { isStatic = value; }
    Material GetMaterial() const { return material; }
    SDL_FColor GetFColorBoundry() const { return boundaryColor; }
    SDL_Color GetColorBoundry() const { return ToColor(boundaryColor); }
    void SetFColorBoundry(SDL_FColor color) { boundaryColor = color; }
    void SetColorBoundry(SDL_Color color) { boundaryColor = ToFColor(color); }

    virtual float GetRadius() const ;
    virtual void SetRadius(float radius) ;

  protected:
    void Render() override = 0;
    void PhysicsUpdate(float dt) override = 0;
    const std::string ShapeTypeToStr(PhysicsShapeType type) const;

    const PhysicsShapeType type;
    bool isStatic;
    float mass;
    float area;
    float rotation;
    float angularVelocity;
    glm::vec2 velocity;
    Material material;
    SDL_FColor boundaryColor;
};