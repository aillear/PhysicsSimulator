#pragma once

#include "FPSCounter.h"
#include "SDL3/SDL_events.h"
#include "eventSystem.h"
#include "object.h"
#include "objectWorld.h"
#include <SDL3_framerate.h>
#include <memory>
#include <string>
#include <vector>
class PhysicsSystem {
  public:
    static PhysicsSystem &Instance();

    void SetRunning() {running = true;}

    bool Init(int targetFPS);
    void Destroy();
    void UpdateWrapper();
    void Update();

    void AddObject(std::shared_ptr<ObjectWorld> obj, std::shared_ptr<ObjectWorld> target = nullptr);
    void AddObject(std::shared_ptr<ObjectWorld> obj, ObjectID targetID);
    void AddObject(std::shared_ptr<ObjectWorld> obj, std::string targetName);

    void RemoveObject(std::shared_ptr<ObjectWorld> obj);
    void RemoveObject(ObjectID objId);
    void RemoveObject(std::string objName);

    std::shared_ptr<ObjectWorld> FindObjectById(ObjectID id);
    std::shared_ptr<ObjectWorld> FindObjectByName(std::string name);



    constexpr static float MinBodySize = 0.01f * 0.01f;
    constexpr static float MaxBodySize = 64.0f * 64.0f;

    constexpr static float MinDensity = 0.25f;
    constexpr static float MaxDensity = 21.4f;

    FPSCounter fpsc;
    private:  
    
    PhysicsSystem() : running(false) {;}
    ~PhysicsSystem() = default;
    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;
    void HandleSDLEvents(SDL_Event& event);
    
    bool running;
    FPSmanager fpsm;
    float targetDt;
    bool hasRemoveCalled = false;
    EventHandlerID eventHandler1_;
    std::shared_ptr<ObjectWorld> rootNode;
    std::vector<std::shared_ptr<ObjectWorld>> physicsObjectsToAdd;
};


# define GET_PhysicsSystem PhysicsSystem::Instance()