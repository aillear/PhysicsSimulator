#include "rigidbody.h"
#include "SDL3/SDL_render.h"
#include "exceptions.h"
#include "logger.h"
#include "physicsSystem.h"
#include <vector>

float RigidBody::GetRadius() const {
    F_LOG_WARNING("Try to get a NoneCircleBody's({}) radius.",
                  ShapeTypeToStr(type_));
    return 0;
}

void RigidBody::SetRadius(float radius) {
    F_LOG_WARNING("Try to set a NoneCircleBody's({}) radius.",
                  ShapeTypeToStr(type_));
    return;
}

const GlmCircle RigidBody::GetCircle() const {
    F_LOG_WARNING("Try to get a NoneCircleBody's({}) circle.",
                  ShapeTypeToStr(type_));
    static GlmCircle __empty_circle__{{0, 0}, 0};
    return __empty_circle__;
}

glm::vec2 RigidBody::GetWidthHeight() const {
    F_LOG_WARNING("Try to get a NoneBoxBody's({}) width and height.",
                  ShapeTypeToStr(type_));
    return {0, 0};
}

void RigidBody::SetWidthHeight(glm::vec2) {
    F_LOG_WARNING("Try to set a NoneBoxBody's({}) width and height.",
                  ShapeTypeToStr(type_));
}

const std::vector<SDL_Vertex> &RigidBody::GetVertex() const {
    F_LOG_WARNING("Try to get a NoneBox/PolygonBody's({}) vertces.",
                  ShapeTypeToStr(type_));
    static std::vector<SDL_Vertex> __empty_vector__;
    return __empty_vector__;
}

const std::string RigidBody::ShapeTypeToStr(PhysicsShapeType type) const {
    switch (type) {
    case PhysicsShapeType::CIRCLE:
        return "CIRCLE";
    case PhysicsShapeType::BOX:
        return "BOX";
    case PhysicsShapeType::POLYGON:
        return "POLYGON";
    default:
        return "UNKNOWED TYPE";
    }
}

bool RigidBody::SafeCheck() const {
    if (area_ < MinBodySize) {
        F_LOG_ERROR("RigidBody area {} is too small, it must be bigger than {}",
                    area_, MinBodySize);
        throw BodyAreaError(area_);
    }
    if (area_ > MaxBodySize) {
        F_LOG_ERROR("RigidBody area {} is too big, it must be smaller than {}",
                    area_, MaxBodySize);
        throw BodyAreaError(area_);
    }
    return true;
}

void RigidBody::OnCollision(RigidBody* rigidBody, glm::vec2 norm, float depth) {
    // not really to use it now;
    // Move(0.5f * norm * depth);
}

void RigidBody::PhysicsUpdate(float dt) {
    if (isStatic_) return;
    // acc is current not use
    // this->velocity_ += force_ * massR_ * dt;
    this->velocity_ += GET_PhysicsSystem.gravity * dt;
     
    Move(velocity_ * dt);
    Rotate(angularVelocity_ * dt);
    force_ = {0, 0};
}

void RigidBody::Rotate(float angle) {
    rotation_ += angle;
    if (rotation_ >= 360.0f) rotation_ -= 360.0f;
    if (rotation_ < 0.0f) rotation_ += 360.0f;
}

void RigidBody::RotateTo(float rotation) {
    if (rotation >= 360.0f) rotation -= 360.0f;
    if (rotation_ < 0.0f) rotation += 360.0f;
    rotation_ = rotation;
}

void RigidBody::SetIsStatic(bool value) {
    if (isStatic_ == value) return;
    if (value == true) massR_ = 0;
    else massR_ = 1.0 / mass_;
    isStatic_ = value;
}