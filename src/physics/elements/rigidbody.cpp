#include "rigidbody.h"
#include "configs.h"
#include "exceptions.h"
#include "logger.h"
#include "physicsSystem.h"
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
    GetVertexTransfrom();
    GetAABBUpdated();
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

void RigidBody::CalRotateIntertia() {
    // not really to use it now;
    throw std::runtime_error("CalRotateIntertia is not implemented for type " +
                             ShapeTypeToStr(type_) + ".");
}

const std::vector<glm::vec2> &RigidBody::GetVertex() const {
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
    if (!isStatic_) {

        // acc is current not use
        // this->velocity_ += force_ * massR_ * dt;
        this->velocity_ += GET_PhysicsSystem.gravity * dt;

        Move(velocity_ * dt);
        RRotate(angularVelocity_ * dt);
        force_ = {0, 0};
    }

    GetVertexTransfrom();
    GetAABBUpdated();
}

void RigidBody::Rotate(float angleDegree) {
    RRotate(glm::radians(angleDegree));
}

void RigidBody::RotateTo(float angleDegree) {
    RRotateTo(glm::radians(angleDegree));
}

void RigidBody::RRotate(float angle) {
    rotation_ += angle;
    if (rotation_ >= MaxRadian)
        rotation_ -= MaxRadian;
    if (rotation_ < MinRadian)
        rotation_ += MaxRadian;
    transformer.SetRAngle(rotation_);

    // if (glm::epsilonEqual(angle, 0.0f, 0.01f))
    //     return;

    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::RRotateTo(float angle) {
    if (angle >= MaxRadian)
        angle -= MaxRadian;
    if (angle < MinRadian)
        angle += MaxRadian;
    transformer.SetRAngle(rotation_);
    // if (glm::epsilonEqual(angle, rotation_, 0.01f))
    //     return;

    rotation_ = angle;
}

void RigidBody::Move(glm::vec2 ds) {
    position_ += ds;
    transformer.SetOffset(position_);
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
    transformer.SetOffset(position_);
    needToTransfrom = true;
    needToUpdateAABB = true;
}

void RigidBody::SetIsStatic(bool value) {
    if (value == true) {
        massR_ = 0;
        rotateIntertiaR_ = 0;
    } else {
        massR_ = 1.0 / mass_;
        rotateIntertiaR_ = 1.0 / rotateIntertia_;
    }
    isStatic_ = value;
}