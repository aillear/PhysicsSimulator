#include "physicsObjectRoot.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include <utility>


void PhysicsObjectRoot::Render() {
    DrawCommand BoundryCommand(ShapeType::HOLLOW_RECT, false);
    BoundryCommand.scaleWithZoom = false;
    BoundryCommand.halfLineWidth = 1.0f;
    BoundryCommand.GetBase().color = {1, 1, 1, 1};
    BoundryCommand.GetBase().rect = {{-38.40f, -21.60f}, {38.40f, 21.60f}};
    GET_Buffer.AddCommand(std::move(BoundryCommand));

    BoundryCommand = DrawCommand(ShapeType::HOLLOW_RECT, false);
    BoundryCommand.scaleWithZoom = false;
    BoundryCommand.halfLineWidth = 1.5f;
    BoundryCommand.GetBase().color = {1, 0, 0, 1};
    BoundryCommand.GetBase().rect = {{-40.00f, -23.20f}, {40.00f, 23.20f}};
    GET_Buffer.AddCommand(std::move(BoundryCommand));

}
void PhysicsObjectRoot::PhysicsUpdate(float dt) {

}