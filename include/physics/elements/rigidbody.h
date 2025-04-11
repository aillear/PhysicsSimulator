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
        : ObjectWorld(position), material_(mate), velocity_({0, 0}), rotation_(0),
          angularVelocity_(0), type_(type) {
            this->SetFColor({1, 1, 1, 1});
            this->SetFColorBoundry({1,1,1,1});
        ;
    }

    void Move(glm::vec2 ds) { position_ += ds; }
    void MoveTo(glm::vec2 destinaion) { position_ = destinaion; }
    PhysicsShapeType GetType() { return type_; }
    float GetMass() const { return mass_; }
    float GetArea() const { return area_; }
    float GetRotation() const { return rotation_; }
    float GetAngularVelocity() const { return angularVelocity_; }
    glm::vec2 GetVelocity() const { return velocity_; }
    void SetVelocity(glm::vec2 v) { velocity_ = v; }
    bool GetIsStatic() { return isStatic_; }
    void SetIsStatic(bool value) { isStatic_ = value; }
    Material GetMaterial() const { return material_; }
    SDL_FColor GetFColorBoundry() const { return boundaryColor_; }
    SDL_Color GetColorBoundry() const { return ToColor(boundaryColor_); }
    virtual void SetFColorBoundry(SDL_FColor color) { boundaryColor_ = color; }
    virtual void SetColorBoundry(SDL_Color color) { boundaryColor_ = ToFColor(color); }

    virtual float GetRadius() const ;
    virtual void SetRadius(float radius) ;

    virtual glm::vec2 GetWidthHeight() const;
    virtual void SetWidthHeight(glm::vec2 wh);

    virtual void SetRotation(float rotation) {rotation_ = rotation;}

  protected:
    void Render() override = 0;
    void PhysicsUpdate(float dt) override = 0;
    const std::string ShapeTypeToStr(PhysicsShapeType type) const;
    bool SafeCheck() const;

    const PhysicsShapeType type_;
    bool isStatic_;
    float mass_;
    float area_;
    float rotation_;
    float angularVelocity_;
    glm::vec2 velocity_;
    Material material_;
    SDL_FColor boundaryColor_;
};