#include "circleBody.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include <utility>

void CircleBody::Render() {
    // draw it self
    DrawCommand cmd(ShapeType::CIRCLE, false);
    cmd.GetBase().color = color;
    cmd.GetBase().circle.center = position_;
    cmd.GetBase().circle.radius = radius_;
    GET_Buffer.AddCommand(std::move(cmd));
    
    // draw outline
    cmd = DrawCommand(ShapeType::HOLLOW_CIRCLE, false);
    cmd.GetBase().color = boundaryColor;
    cmd.GetBase().circle.center = position_;
    cmd.GetBase().circle.radius = radius_;
    cmd.halfLineWidth = 0.5f;
    GET_Buffer.AddCommand(std::move(cmd));

}

void CircleBody::PhysicsUpdate(float dt) {

}