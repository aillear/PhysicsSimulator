#pragma once

#include "SDL3/SDL_pixels.h"
#include "configs.h"
#include "conversion.h"
#include "material.h"
#include "objectWorld.h"
#include "renderSystem.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>

// TODO: add more physics properties
// each pxiel represen 1 cm.
class RigidBody : public ObjectWorld {
  public:
    RigidBody(glm::vec2 position, Material mate, PhysicsShapeType type);

    virtual void Move(glm::vec2 ds) { position_ += ds; }
    virtual void MoveTo(glm::vec2 destinaion) { position_ = destinaion; }
    virtual void Rotate(float angle);
    virtual void RotateTo(float rotation);
    PhysicsShapeType GetType() { return type_; }

    const float GetMass() const { return mass_; }
    const float GetMassR() const { return massR_; }
    const float GetArea() const { return area_; }
    const float GetRotation() const { return rotation_; }
    const float GetAngularVelocity() const { return angularVelocity_; }
    const float GetRotateIntertia() const { return rotateIntertia_; }
    const float GetRotateIntertiaR() const { return rotateIntertiaR_; }
    Material GetMaterial() const { return material_; }
    glm::vec2 GetVelocity() const { return velocity_; }

    void SetVelocity(glm::vec2 v) { velocity_ = v; }
    void AddVelocity(glm::vec2 v) { velocity_ += v; }

    void AddForce(glm::vec2 force) { this->force_ = force; }
    const glm::vec2 GetForce() { return force_; }

    bool GetIsStatic() { return isStatic_; }
    void SetIsStatic(bool value);

    SDL_FColor GetFColorBoundry() const { return boundaryColor_; }
    SDL_Color GetColorBoundry() const { return ToColor(boundaryColor_); }
    virtual void SetFColorBoundry(SDL_FColor color) { boundaryColor_ = color; }
    virtual void SetColorBoundry(SDL_Color color) {
        boundaryColor_ = ToFColor(color);
    }

    const PhysicsShapeType GetPhysicsType() const { return type_; }

    virtual void OnCollision(RigidBody *rigidBody, glm::vec2 norm, float depth);

    // circle
    virtual float GetRadius() const;
    virtual void SetRadius(float radius);
    virtual const GlmCircle GetCircle() const;

    virtual glm::vec2 GetWidthHeight() const;
    virtual void SetWidthHeight(glm::vec2 wh);

    virtual const std::vector<SDL_Vertex> &GetVertex() const;

    virtual const AABB GetAABB() const { return aabb_; }

    // for universal physics
    const float GetUMass() const { return mass_; }
    const float GetUArea() const { return area_; }
    const glm::vec2 GetUVelocity() const {
        return velocity_ * TULongitudeFactor;
    }
    const glm::vec2 GetUPosition() const {
        return position_ * TULongitudeFactor;
    }

  protected:
    void Init() override;
    void Render() override = 0;
    void PhysicsUpdate(float dt) override;
    const std::string ShapeTypeToStr(PhysicsShapeType type) const;
    bool SafeCheck() const;

    virtual void GetVertexTransfrom() { needToTransfrom = false; }

    virtual void GetAABBUpdated() { needToUpdateAABB = false; }

    virtual void CalRotateIntertia();

    const PhysicsShapeType type_;
    bool needToTransfrom = true;
    bool needToUpdateAABB = true;
    bool isStatic_;
    float mass_;
    float massR_;
    float area_;
    float rotation_;
    float angularVelocity_;
    float rotateIntertia_; // moment of inertia
    float rotateIntertiaR_; // moment of inertia inverse
    glm::vec2 velocity_;
    glm::vec2 force_;
    AABB aabb_;

    Material material_;
    SDL_FColor boundaryColor_;
};