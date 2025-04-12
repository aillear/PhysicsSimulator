#include "physicsObjectRoot.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include <utility>


void PhysicsObjectRoot::Render() {
    DrawCommand BoundryCommand(ShapeType::HOLLOW_RECT, false);
    BoundryCommand.scaleWithZoom = false;
    BoundryCommand.halfLineWidth = 1.0f;
    BoundryCommand.GetBase().color = {1, 1, 1, 1};
    BoundryCommand.GetBase().rect = {{-3840, -2160}, {3840, 2160}};
    GET_Buffer.AddCommand(std::move(BoundryCommand));
}
void PhysicsObjectRoot::PhysicsUpdate(float dt) {

}