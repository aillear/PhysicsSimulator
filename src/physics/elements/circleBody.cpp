#include "circleBody.h"
#include "configs.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include <numbers>
#include <utility>

CircleBody::CircleBody(Material mate, glm::vec2 center, float radius)
    : RigidBody(center, mate, PhysicsShapeType::CIRCLE), radius_(radius) {
    area_ = std::numbers::pi * radius * radius * TUAreaFactor;
    mass_ = area_ * material_.density;
    massR_ = 1.0 / mass_;
    SetFColor({1,1,0,1});
    SetFColorBoundry({1,1,1,1});
}

const GlmCircle CircleBody::GetCircle() const  {
    return {position_, radius_};
}

void CircleBody::Render() {
    // draw it self
    DrawCommand cmd(ShapeType::CIRCLE, false);
    cmd.GetBase().color = color_;
    cmd.GetBase().circle = GetCircle();
    GET_Buffer.AddCommand(std::move(cmd));

    // draw outline
    cmd = DrawCommand(ShapeType::HOLLOW_CIRCLE, false);
    cmd.GetBase().color = boundaryColor_;
    cmd.GetBase().circle = GetCircle();
    cmd.halfLineWidth = 0.5f;
    GET_Buffer.AddCommand(std::move(cmd));
}

void CircleBody::Move(glm::vec2 ds) {
    SetPosition(position_ + ds);
    needToUpdateAABB = true;
}

void CircleBody::MoveTo(glm::vec2 destination) {
    SetPosition(destination);
    needToUpdateAABB = true;
}

void CircleBody::GetAABBUpdated() {
    if (!needToUpdateAABB) return;

    aabb_.maxP = {position_.x + radius_, position_.y + radius_};
    aabb_.minP = {position_.x - radius_, position_.y - radius_};
    needToUpdateAABB = false;
}

void CircleBody::PhysicsUpdate(float dt) {
    RigidBody::PhysicsUpdate(dt);
}