#include "physicsSystem.h"
#include "SDL3/SDL_events.h"
#include "eventSystem.h"
#include <algorithm>

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