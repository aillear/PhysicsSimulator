#pragma once

#include "SDL3/SDL_scancode.h"
#include "collisionMgr.h"
#include "inputSystem.h"
#include "logger.h"
#include "objectWorld.h"
#include "rigidbody.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
class PhysicsObjectRoot : public ObjectWorld {
  protected:
    void Init() override { position_ = {0, 0}; }
    void Render() override { ; }
    void PhysicsUpdate(float dt) override {
        glm::vec2 dir = {0, 0};

        if (KeyState(SDL_SCANCODE_W))
            dir.y--;
        if (KeyState(SDL_SCANCODE_S))
            dir.y++;
        if (KeyState(SDL_SCANCODE_A))
            dir.x--;
        if (KeyState(SDL_SCANCODE_D))
            dir.x++;

        if (dir.x != 0 || dir.y != 0) {
            auto firstObj = static_cast<RigidBody *>(children.begin()->get());
            if (firstObj == nullptr)
                return;
            firstObj->Move(glm::normalize(dir) * 100.0f * dt);
        }

        // int childCount = children.size();
        // glm::vec2 norm;
        // float depth;
        // for (int i = 0; i < childCount - 1; i++) {
        //     for (int j = i + 1; j < childCount; j++) {
        //         auto a = (RigidBody*)children[i].get();
        //         auto b = (RigidBody*)children[j].get();
        //         if (!GET_CollisionMgr.IntersectCircle(
        //                 {a->GetPosition(), a->GetRadius()},
        //                 {b->GetPosition(), b->GetRadius()}, norm, depth))
        //             continue;
        //         a->Move(norm * depth * -0.5f);
        //         b->Move(norm * depth * 0.5f);
        //     }
        // }

        int childCount = children.size();
        for (auto &child : children) {
            auto temp = (RigidBody *)child.get();
            temp->SetFColorBoundry({1, 1, 1, 1});
        }

        for (int i = 0; i < childCount - 1; i++) {
            for (int j = i + 1; j < childCount; j++) {
                auto a = (RigidBody *)children[i].get();
                auto b = (RigidBody *)children[j].get();
                if (!GET_CollisionMgr.IntersectPolygon(a->GetVertex(),
                                                       b->GetVertex()))
                    continue;
                a->SetFColorBoundry({1, 0, 0, 1});
                b->SetFColorBoundry({1, 0, 0, 1});
            }
        }
    }
};