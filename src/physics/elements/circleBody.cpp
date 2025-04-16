#include "circleBody.h"
#include "configs.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include <numbers>
#include <utility>

CircleBody::CircleBody(Material mate, float radius)
    : RigidBody(mate, PhysicsShapeType::CIRCLE), radius_(radius) {
    area_ = std::numbers::pi * radius * radius * TUAreaFactor;
    mass_ = area_ * material_.density * TUMassFactor;
    massR_ = 1.0 / mass_;
    
    SetFColor({1, 1, 0, 1});
    SetFColorBoundry({1, 1, 1, 1});
}

const GlmCircle CircleBody::GetCircle() const { return {position_, radius_}; }

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


void CircleBody::GetAABBUpdated() {
    if (!needToUpdateAABB)
        return;

    aabb_.maxP = {position_.x + radius_, position_.y + radius_};
    aabb_.minP = {position_.x - radius_, position_.y - radius_};
    needToUpdateAABB = false;
}

void CircleBody::CalRotateIntertia() {
    rotateIntertia_ = 0.5f * mass_ * radius_ * radius_;
    rotateIntertiaR_ = 1.0f / rotateIntertia_;
}



void CircleBody::PhysicsUpdate(float dt) { RigidBody::PhysicsUpdate(dt); }