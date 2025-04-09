#include "physicsSystem.h"
#include "SDL3/SDL_events.h"
#include "eventSystem.h"
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
    
    eventHandler1_ = GET_EventSystem.AddEventListener(SDL_EVENT_QUIT, [this](SDL_Event&){
        this->running = false;
    });
    
    return true;
}

void PhysicsSystem::Destroy() {
    GET_EventSystem.RemoveEventListener(SDL_EVENT_QUIT, eventHandler1_);
}

void PhysicsSystem::Update() {
}

void PhysicsSystem::AddObject(std::shared_ptr<RigidBody> obj, std::shared_ptr<RigidBody> target) {
    obj->SetParent(target.get());
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<RigidBody> obj, Uint32 targetID) {

}

void PhysicsSystem::AddObject(std::shared_ptr<RigidBody> obj, std::string targetName) {
;
}

void PhysicsSystem::RemoveObject(std::shared_ptr<RigidBody> obj) {
    // remove object
    



}

void PhysicsSystem::RemoveObject(Uint32 objID) {

}