#include "rigidbody.h"
#include "logger.h"
#include "physicsSystem.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <memory>

void RigidBody::Render() {
    switch (type) {
        case PhysicsShapeType::CIRCLE: {
            DrawCommand cmd(ShapeType::HOLLOW_CIRCLE, false);
            cmd.GetBase().circle.center = position_;
            cmd.GetBase().circle.radius = radius;
            cmd.GetBase().color = color;
            cmd.halfLineWidth = 1.0f;
            GET_Buffer.AddCommand(std::move(cmd));
            break;
        }
        case PhysicsShapeType::BOX: {
            DrawCommand cmd(ShapeType::HOLLOW_RECT, false);
            cmd.GetBase().rect.p1 = position_;
            cmd.GetBase().rect.p2 = widthHeight + position_;
            cmd.GetBase().color = color;
            cmd.halfLineWidth = 1.0f;
            GET_Buffer.AddCommand(std::move(cmd));
            break;
        }
        default:
            break;
    }
}

void RigidBody::PhysicsUpdate(float dt) {

}


std::shared_ptr<RigidBody> CreateCircleBody(float radius, glm::vec2 position,
                                            Material mate) {

    float area = M_PI * radius * radius;
    if (area < PhysicsSystem::MinBodySize) {
        F_LOG_ERROR("Circle radius is too small, Min body size is {}.",
                    PhysicsSystem::MinBodySize);
        return nullptr;
    } else if (area > PhysicsSystem::MaxBodySize) {
        F_LOG_ERROR("Circle radius is too big, Max body size is {}.",
                    PhysicsSystem::MaxBodySize);
        return nullptr;
    }

    if (mate.density < PhysicsSystem::MinDensity) {
        F_LOG_ERROR("Circle density is too small, Min density is {}",
                    PhysicsSystem::MinDensity);
        return nullptr;
    } else if (mate.density > PhysicsSystem::MaxDensity) {
        F_LOG_ERROR("Circle density is too big, Min density is {}",
                    PhysicsSystem::MaxDensity);
        return nullptr;
    }

    float mass = area * mate.density;

    auto obj = std::make_shared<RigidBody>(position, area, mate, glm::vec2{0, 0},
                                       radius, false, PhysicsShapeType::CIRCLE);
    obj->SetColor({255, 0, 0, 255});
    return obj;
}

std::shared_ptr<RigidBody> CreateBoxBody(glm::vec2 wh, glm::vec2 position,
                                         Material mate) {

    float area = wh.x * wh.y;
    if (area < PhysicsSystem::MinBodySize) {
        F_LOG_ERROR("box area is too small, Min body size is {}.",
                    PhysicsSystem::MinBodySize);
        return nullptr;
    } else if (area > PhysicsSystem::MaxBodySize) {
        F_LOG_ERROR("box area is too big, Max body size is {}.",
                    PhysicsSystem::MaxBodySize);
        return nullptr;
    }

    if (mate.density < PhysicsSystem::MinDensity) {
        F_LOG_ERROR("box density is too small, Min density is {}",
                    PhysicsSystem::MinDensity);
        return nullptr;
    } else if (mate.density > PhysicsSystem::MaxDensity) {
        F_LOG_ERROR("box density is too big, Min density is {}",
                    PhysicsSystem::MaxDensity);
        return nullptr;
    }

    float mass = area * mate.density;

    auto obj = std::make_shared<RigidBody>(position, area, mate, wh,
                                       0, false, PhysicsShapeType::BOX);
    obj->SetColor({0, 255, 0, 255});
    return obj;
}
