#include "physicsSystem.h"
#include "SDL3/SDL_events.h"
#include "collisionMgr.h"
#include "eventSystem.h"
#include "logger.h"
#include "object.h"
#include "objectWorld.h"
#include "physicsObjectRoot.h"
#include "renderBufferMgr.h"
#include "rigidbody.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <string>

using Shape = PhysicsShapeType;

PhysicsSystem &PhysicsSystem::Instance() {
    static PhysicsSystem instance;
    return instance;
}

bool PhysicsSystem::Init(int targetFrame) {

    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 100);
    targetDt = 1.0f / targetFrame;

    rootNode = std::make_shared<PhysicsObjectRoot>();
    rootNode->SetName("rootPhysicsNode");   

    // add event handler, subscribe 6 event
    eventHandler_1 = GET_EventSystem.AddEventListener(
        SDL_EVENT_QUIT, [this](SDL_Event &) { this->running = false; });

    eventHandler_2 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    eventHandler_3 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    eventHandler_4 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_UP,
        [this](SDL_Event &event) { HandleSDLEvents(event); });
    return true;
}

void PhysicsSystem::Destroy() {
    // release all event
    GET_EventSystem.RemoveEventListener(SDL_EVENT_QUIT, eventHandler_1);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_MOTION, eventHandler_2);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN,
                                        eventHandler_3);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_UP,
                                        eventHandler_4);
}

void PhysicsSystem::UpdateWrapper() {
    for (auto &callBack : initFunctionWrapper)
        callBack();
    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        Update();
        CollisionHandler();
        for (auto& callBack : AfterUpdateFunctionWrapper) callBack();
        GET_Buffer.Submit();
        fpsc.EndFrame();
    }
    LOG_INFO("physics cycle is end.");
}

void PhysicsSystem::Update() {
    if (hasRemoveCalled) {
        rootNode->CheckChildToRemove();
        hasRemoveCalled = false;
    }

    for (auto &obj : physicsObjectsToAdd) {
        auto parent = obj->GetParent();
        if (parent == nullptr) {
            rootNode->AddChild(obj);
        } else
            parent->AddChild(obj);
        obj->InitWrapper();
    }
    physicsObjectsToAdd.clear();

    rootNode->PhysicsUpdateWrapper(targetDt);
    rootNode->RenderWrapper();
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj,
                              std::shared_ptr<ObjectWorld> target) {
    obj->SetParent(target.get());
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj,
                              ObjectID targetID) {
    Object *target = FindObjectById(targetID).get();
    obj->SetParent(target);
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj,
                              std::string targetName) {
    Object *target = FindObjectByName(targetName).get();
    obj->SetParent(target);
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::RemoveObject(std::shared_ptr<ObjectWorld> obj) {
    obj->SetToRemove();
    hasRemoveCalled = true;
}

void PhysicsSystem::RemoveObject(ObjectID name) {
    auto removeObj = FindObjectById(name);
    removeObj->SetToRemove();
    hasRemoveCalled = false;
}

void PhysicsSystem::RemoveObject(std::string name) {
    auto removeObj = FindObjectByName(name);
    removeObj->SetToRemove();
    hasRemoveCalled = false;
}

void PhysicsSystem::HandleSDLEvents(SDL_Event &event) {
    rootNode->HandleEventWrapper(event);
}

std::shared_ptr<ObjectWorld> PhysicsSystem::FindObjectById(ObjectID id) {
    for (auto &comp : physicsObjectsToAdd) {
        if (comp->GetID() == id)
            return comp;
    }
    return std::static_pointer_cast<ObjectWorld>(rootNode->GetChildByID(id));
}

std::shared_ptr<ObjectWorld> PhysicsSystem::FindObjectByName(std::string name) {
    for (auto &comp : physicsObjectsToAdd) {
        if (comp->GetName() == name)
            return comp;
    }
    return std::static_pointer_cast<ObjectWorld>(
        rootNode->GetChildByName(name));
}

void PhysicsSystem::CollisionHandler() {
    auto children = rootNode->GetChildren();
    int childCount = children.size();
    glm::vec2 norm{0, 0};
    float depth = 0;
    for (int i = 0; i < childCount-1; i++) {
        RigidBody* objA = static_cast<RigidBody*>(children[i].get());
        for (int j = i+1; j < childCount; j++) {
            RigidBody* objB = static_cast<RigidBody*>(children[j].get());
            
            if (!Collision(objA, objB, norm, depth)) continue;
            
            // objA->OnCollision(objB, norm, depth);
            // objB->OnCollision(objB, norm, depth);
            norm *= depth * 0.5f;
            objA->Move(-norm);
            objB->Move(norm); 
        }
    }
}

bool PhysicsSystem::Collision(RigidBody* a, RigidBody* b, glm::vec2& norm, float& depth) {
    norm = {0, 0};
    depth = 0;

    Shape typeA = a->GetPhysicsType();
    Shape typeB = b->GetPhysicsType();

    if (typeA == Shape::CIRCLE) {
        if (typeB == Shape::CIRCLE) {
            return GET_CollisionMgr.IntersectCircle(a->GetCircle(), b->GetCircle(), norm, depth);
        }
        else {
            return GET_CollisionMgr.IntersectPolygonAndCircle(a->GetCircle(), b->GetVertex(), norm, depth);
        }
    }
    else {
        if (typeB == Shape::CIRCLE) {
            bool result = GET_CollisionMgr.IntersectPolygonAndCircle(b->GetCircle(), a->GetVertex(), norm, depth);
            norm = - norm;
            return result;
        }
        else {
            return GET_CollisionMgr.IntersectPolygon(a->GetVertex(), b->GetVertex(), norm, depth);
        }
    }

}