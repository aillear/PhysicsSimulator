#include "rigidbody.h"
#include "configs.h"
#include "exceptions.h"
#include "logger.h"
#include "physicsSystem.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

RigidBody::RigidBody(Material mate, PhysicsShapeType type)
    : ObjectWorld(), material_(mate), velocity_(0, 0), rotation_(0),
      angularVelocity_(0), type_(type), force_(0, 0) {}
void RigidBody::Init() {
    SafeCheck();
    needToTransfrom = true;
    needToUpdateAABB = true;
}

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

void RigidBody::CalRotateInertia() {
    // not really to use it now;
    throw std::runtime_error("CalRotateIntertia is not implemented for type " +
                             ShapeTypeToStr(type_) + ".");
}

const std::vector<glm::vec2> &RigidBody::GetVertices() {
    F_LOG_WARNING("Try to get a NoneBox/PolygonBody's({}) vertces.",
                  ShapeTypeToStr(type_));
    static std::vector<glm::vec2> __empty_vector__;
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

void RigidBody::OnCollision(RigidBody *rigidBody, glm::vec2 norm, float depth) {
    // not really to use it now;
    // Move(0.5f * norm * depth);
}

void RigidBody::PhysicsUpdate(float dt) {
    if (isStatic_)
        return;

    // acc is current not use
    // this->velocity_ += force_ * massR_ * dt;
    this->velocity_ += GET_PhysicsSystem.gravity * dt;

    position_ += velocity_ * dt;
    rotation_ += angularVelocity_ * dt;
    NormalizeRotation();
    force_ = {0, 0};

    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::RotateDegrees(float angleDegree) {
    Rotate(glm::radians(angleDegree));
}

void RigidBody::RotateToDegrees(float angleDegree) {
    RotateTo(glm::radians(angleDegree));
}

void RigidBody::Rotate(float angle) {
    rotation_ += angle;
    NormalizeRotation();

    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::RotateTo(float angle) {
    rotation_ = angle;
    NormalizeRotation();

    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::NormalizeRotation() {
    if (rotation_ > glm::two_pi<float>()) {
        rotation_ -= glm::two_pi<float>();
    } else if (rotation_ < 0) {
        rotation_ += glm::two_pi<float>();
    }
}

void RigidBody::Move(glm::vec2 ds) {
    position_ += ds;
    // if (glm::all(glm::epsilonEqual(ds, {0, 0}, 0.0001f))) {
    //     return;
    // }
    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::MoveTo(glm::vec2 destination) {
    // if (glm::all(glm::epsilonEqual(destination, position_, 0.0001f))) {
    //     return;
    // }
    position_ = destination;
    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::ApplyImpulsePoint(glm::vec2 impulse, glm::vec2 point) {
    if (isStatic_) return;

    velocity_ += impulse * massR_;
    angularVelocity_ += Cross(point - position_, impulse) * rotateInertiaR_;
}

void RigidBody::ApplyImpulse(glm::vec2 impulse, glm::vec2 r) {
    if (isStatic_) return;

    velocity_ += impulse * massR_;
    angularVelocity_ += Cross(r, impulse) * rotateInertiaR_;
}


glm::vec2 RigidBody::LocalToWorld(glm::vec2 local) {
    transformer.Reset(rotation_, position_);
    return transformer.TransfromR(local);
}

void RigidBody::SetIsStatic(bool value) {
    if (value == true) {
        massR_ = 0;
        rotateInertiaR_ = 0;
    } else {
        massR_ = 1.0 / mass_;
        rotateInertiaR_ = 1.0 / rotateInertia_;
    }
    isStatic_ = value;
}
