#pragma once

#include "FPSCounter.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include "eventSystem.h"
#include "rigidbody.h"
#include <SDL3_framerate.h>
#include <memory>
#include <string>
#include <vector>
class PhysicsSystem {
  public:
    static PhysicsSystem &Instance();
    bool Init(int targetFPS);
    void Destroy();
    void Update();

    void AddObject(std::shared_ptr<RigidBody> obj, std::shared_ptr<RigidBody> target = nullptr);
    void AddObject(std::shared_ptr<RigidBody> obj, Uint32 targetID);
    void AddObject(std::shared_ptr<RigidBody> obj, std::string targetName);

    void RemoveObject(std::shared_ptr<RigidBody> obj);
    void RemoveObject(Uint32 objId);
    void RemoveObject(std::string objName);


    constexpr static float MinBodySize = 0.01f * 0.01f;
    constexpr static float MaxBodySize = 64.0f * 64.0f;

    constexpr static float MinDensity = 0.25f;
    constexpr static float MaxDensity = 21.4f;

    FPSCounter fpsc;
    bool running;
  private:  
    PhysicsSystem() : running(false) {;}
    ~PhysicsSystem() = default;
    FPSmanager fpsm;
    float targetDt;
    EventHandlerID eventHandler1_;

    
    std::vector<std::shared_ptr<RigidBody>> physicsObjectsToAdd;

};


# define GET_PhysicsSystem PhysicsSystem::Instance()