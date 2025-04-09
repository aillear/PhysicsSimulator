#include "physicsSystem.h"
#include "SDL3/SDL_events.h"
#include "eventSystem.h"
#include "logger.h"
#include "material.h"
#include "object.h"
#include "objectWorld.h"
#include "PhysicsObjectRoot.h"
#include "renderBufferMgr.h"
#include "rigidbody.h"
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
    
    rootNode = std::make_shared<PhysicsObjectRoot>();
    rootNode->SetName("rootPhysicsNode");
    
    eventHandler1_ = GET_EventSystem.AddEventListener(SDL_EVENT_QUIT, [this](SDL_Event&){
        this->running = false;
    });

    Material m{1, 1, 0.5};
    for(int i = 0; i < 10; i++) {
        AddObject(CreateCircleBody(20, {i*100%500, (i < 5) ? 0 : 100},  m));
    }
    
    return true;
}

void PhysicsSystem::Destroy() {
    GET_EventSystem.RemoveEventListener(SDL_EVENT_QUIT, eventHandler1_);
}

void PhysicsSystem::UpdateWrapper() {
    while (running) {
        fpsc.StartFrame();
        SDL_framerateDelay(&fpsm);
        Update();
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

    for (auto& obj : physicsObjectsToAdd) {
        auto parent = obj->GetParent();
        if (parent == nullptr) {
            rootNode->AddChild(obj);
        }
        else parent->AddChild(obj);
        obj->InitWrapper();
    }
    physicsObjectsToAdd.clear();

    rootNode->PhysicsUpdateWrapper(targetDt);
    rootNode->RenderWrapper();
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj, std::shared_ptr<ObjectWorld> target) {
    obj->SetParent(target.get());
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj, ObjectID targetID) {
    Object* target = FindObjectById(targetID).get();
    obj->SetParent(target);
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj, std::string targetName) {
    Object* target = FindObjectByName(targetName).get();
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

void PhysicsSystem::HandleSDLEvents(SDL_Event& event) {

}

std::shared_ptr<ObjectWorld> PhysicsSystem::FindObjectById(ObjectID id) {
    for (auto& comp: physicsObjectsToAdd) {
        if (comp->GetID() == id) return comp;
    }
    return std::static_pointer_cast<ObjectWorld>(rootNode->GetChildByID(id));

}

std::shared_ptr<ObjectWorld> PhysicsSystem::FindObjectByName(std::string name) {
    for (auto& comp: physicsObjectsToAdd) {
        if (comp->GetName() == name) return comp;
    }
    return std::static_pointer_cast<ObjectWorld>(rootNode->GetChildByName(name));
}