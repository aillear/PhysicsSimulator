#include "circleBody.h"
#include "configs.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include "shape.h"
#include <numbers>
#include <utility>

CircleBody::CircleBody(Material mate, float radius)
    : RigidBody(mate, PhysicsShapeType::CIRCLE), radius_(radius) {
    area_ = std::numbers::pi * radius * radius;
    mass_ = area_ * material_.density * TUMassFactor;
    massR_ = 1.0 / mass_;
    CalRotateIntertia();

    SetFColor({1, 1, 0, 1});
    SetFColorBoundry({1, 1, 1, 1});
    originalDrawLine_ = {position_, position_ + glm::vec2(radius_, 0)};
}

const GlmCircle CircleBody::GetCircle() const { return {position_, radius_}; }

void CircleBody::Render() {
    // draw it self
    DrawCommand cmd(ShapeType::CIRCLE, false);
    cmd.GetBase().color = color_;
    cmd.GetBase().circle = GetCircle();
    GET_Buffer.AddCommand(std::move(cmd));

    cmd = DrawCommand(ShapeType::LINE, false);
    cmd.GetBase().color = boundaryColor_;
    cmd.GetBase().rect = GetDrawline();
    cmd.halfLineWidth = 0.75f;
    GET_Buffer.AddCommand(std::move(cmd));

    // draw outline
    cmd = DrawCommand(ShapeType::HOLLOW_CIRCLE, false);
    cmd.GetBase().color = boundaryColor_;
    cmd.GetBase().circle = GetCircle();
    cmd.halfLineWidth = 0.5f;
    GET_Buffer.AddCommand(std::move(cmd));
}

const AABB &CircleBody::GetAABB() {
    if (!needToUpdateAABB)
        return aabb_;

    needToUpdateAABB = false;
    aabb_.maxP = {position_.x + radius_, position_.y + radius_};
    aabb_.minP = {position_.x - radius_, position_.y - radius_};
    return aabb_;
}

GlmRect CircleBody::GetDrawline() {
    if (!needToTransfrom) {
        return drawLine_;
    }
    needToTransfrom = false;
    transformer.Reset(rotation_, position_);
    drawLine_.p1 = transformer.TransfromR(originalDrawLine_.p1);
    drawLine_.p2 = transformer.TransfromR(originalDrawLine_.p2);
    return drawLine_;
}

void CircleBody::CalRotateIntertia() {
    rotateIntertia_ = 0.5f * mass_ * radius_ * radius_;
    rotateIntertiaR_ = 1.0f / rotateIntertia_;
}

void CircleBody::PhysicsUpdate(float dt) { RigidBody::PhysicsUpdate(dt); }