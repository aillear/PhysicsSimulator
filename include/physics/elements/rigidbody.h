#pragma once

#include "SDL3/SDL_pixels.h"
#include "conversion.h"
#include "material.h"
#include "objectWorld.h"
#include "renderSystem.h"
#include "shape.h"
#include "transform.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <string>

// TODO: add more physics properties
// each pxiel represen 1 cm.
// 1 pxiel in screen(origin zoom rate) = 1 cm = 0.01 m
// so that you the data stored here are meter

class RigidBody : public ObjectWorld {
  public:
    RigidBody(Material mate, PhysicsShapeType type);

    virtual void Move(glm::vec2 ds);
    virtual void MoveTo(glm::vec2 destination);
    void SetVelocity(glm::vec2 v) { velocity_ = v; }
    void AddVelocity(glm::vec2 v) { velocity_ += v; }

    virtual void Rotate(float angleDegrees);
    virtual void RotateTo(float angleDegrees);
    virtual void RRotate(float angle);
    virtual void RRotateTo(float angle);
    void SetAngularVelocity(float vDegrees) { angularVelocity_ = glm::radians(vDegrees); }
    void SetRAngularVelocity(float v) { angularVelocity_ = v; }
    void AddAngularVelocity(float vDegrees) { angularVelocity_ += glm::radians(vDegrees); }
    void AddRAngularVelocity(float v) { angularVelocity_ += v; }


    void AddForce(glm::vec2 force) { this->force_ = force; }
    const glm::vec2 GetForce() { return force_; }
    
    PhysicsShapeType GetType() { return type_; }
    const float GetMass() const { return mass_; }
    const float GetMassR() const { return massR_; }
    const float GetArea() const { return area_; }
    const float GetRotation() const { return glm::degrees(rotation_); }
    const float GetRRotation() const { return rotation_; }
    const float GetAngularVelocity() const { return glm::degrees(angularVelocity_); }
    const float GetRAngularVelocity() const { return angularVelocity_; }
    const float GetRotateIntertia() const { return rotateIntertia_; }
    const float GetRotateIntertiaR() const { return rotateIntertiaR_; }
    Material GetMaterial() const { return material_; }
    glm::vec2 GetVelocity() const { return velocity_; }

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

    virtual const std::vector<glm::vec2> &GetVertex() const;

    virtual const AABB GetAABB() const { return aabb_; }


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
    Transform2D transformer;

    private:
    // hide these functions
    void SetPosition(const glm::vec2 &pos) override{;}
    void SetPosition(float x, float y) override{;}
};