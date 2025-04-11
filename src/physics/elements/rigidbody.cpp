#include "rigidbody.h"
#include "logger.h"
#include "shape.h"
#include <string>

float RigidBody::GetRadius() const {
    F_LOG_WARNING("Try to get a NoneCircleBody's({}) radius", ShapeTypeToStr(type));
    return 0;
}

void RigidBody::SetRadius(float radius) {
    F_LOG_WARNING("Try to set a NoneCircleBody's({}) radius", ShapeTypeToStr(type));
    return;
}

const std::string RigidBody::ShapeTypeToStr(PhysicsShapeType type) const {
    switch (type) {
        case PhysicsShapeType::CIRCLE:  return "CIRCLE";
        case PhysicsShapeType::BOX:     return "BOX";
        case PhysicsShapeType::POLYGON: return "POLYGON";
        default:                        return "UNKNOWED TYPE";
    }
}