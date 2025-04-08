#include "physicsSystem.h"
#include "SDL3/SDL_events.h"
#include "eventSystem.h"
#include "object.h"
#include "rigidbody.h"
#include <algorithm>
#include <memory>
#include <string>

PhysicsSystem &PhysicsSystem::Instance() {
    static PhysicsSystem instance;
    return instance;
}

bool PhysicsSystem::Init(int targetFrame) {

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 100);
    targetDt = 1.0f / targetFrame;
    
    GET_EventSystem.AddEventListener(SDL_EVENT_QUIT, [this](){
        this->running = false;
    });
    
    return true;
}

void PhysicsSystem::Destroy() {;}

void PhysicsSystem::Update() {
    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        for (auto &obj : physicsObjectsToAdd) {
            auto parent = obj->GetParent();
            if (parent == nullptr) {
                physicsObjects.emplace_back(obj);
            } else
                parent->AddChild(obj);
            obj->InitWrapper();
        }
        physicsObjectsToAdd.clear();

        for (auto &obj : physicsObjectsToRemove) {
            auto parent = obj->GetParent();
            if (parent == nullptr) {
                auto it = std::remove(physicsObjects.begin(),
                                      physicsObjects.end(), obj);
                if (it != physicsObjects.end()) {
                    physicsObjects.erase(it, physicsObjects.end());
                }
            } else
                obj->GetParent()->RemoveChild(obj);

            obj->DestroyWrapper();
        }
        physicsObjectsToRemove.clear();

        for (auto &obj : physicsObjects) {
            if (obj->GetEnabled())
                obj->PhysicsUpdateWrapper(targetDt);
        }
        fpsc.EndFrame();
    }
}

void PhysicsSystem::AddObject(std::shared_ptr<RigidBody> obj, std::shared_ptr<RigidBody> target) {
    obj->SetParent(target.get());
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<RigidBody> obj, Uint32 targetID) {
    RigidBody* target = nullptr;
    for (auto& obj : physicsObjects) {
        target = static_cast<RigidBody*>(obj->GetChildByID(targetID).get());
        if (target != nullptr) {
            break;
        }
    }
    if (target == nullptr) {
        for (auto& obj: physicsObjectsToAdd) {
            target = static_cast<RigidBody*>(obj->GetChildByID(targetID).get());
            if (target != nullptr) break;
        }
    }
    obj->SetParent(target);
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<RigidBody> obj, std::string targetName) {
    RigidBody* target = nullptr;
    for (auto& obj : physicsObjects) {
        target = static_cast<RigidBody*>(obj->GetChildByName(targetName).get());
        if (target != nullptr) {
            break;
        }
    }
    if (target == nullptr) {
        for (auto& obj: physicsObjectsToAdd) {
            target = static_cast<RigidBody*>(obj->GetChildByName(targetName).get());
            if (target != nullptr) break;
        }
    }
    obj->SetParent(target);
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::RemoveObject(std::shared_ptr<RigidBody> obj) {
    // remove object
    



}

void PhysicsSystem::RemoveObject(Uint32 objID) {

}