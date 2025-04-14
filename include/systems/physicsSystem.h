#pragma once

#include "FPSCounter.h"
#include "SDL3/SDL_events.h"
#include "configs.h"
#include "eventSystem.h"
#include "object.h"
#include "rigidbody.h"
#include <SDL3_framerate.h>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <string>
#include <vector>

class PhysicsSystem {
  public:
    static PhysicsSystem &Instance();

    void SetRunning() { running = true; }

    bool Init(int iteration);
    void Destroy();
    void UpdateWrapper();
    void ObjManage();

    const std::shared_ptr<ObjectWorld> GetRootNode() { return rootNode; }

    void AddObject(std::shared_ptr<ObjectWorld> obj,
                   std::shared_ptr<ObjectWorld> target = nullptr);
    void AddObject(std::shared_ptr<ObjectWorld> obj, ObjectID targetID);
    void AddObject(std::shared_ptr<ObjectWorld> obj, std::string targetName);

    void RemoveObject(std::shared_ptr<Object> obj);
    void RemoveObject(ObjectID objId);
    void RemoveObject(std::string objName);

    std::shared_ptr<ObjectWorld> FindObjectById(ObjectID id);
    std::shared_ptr<ObjectWorld> FindObjectByName(std::string name);

    const int GetBodyCount() const { return rootNode->GetChildren().size(); }

    void AddCustomInitFunction(BasicFunctionWrapper callBack) {
        initFunctionWrapper.emplace_back(std::move(callBack));
    }
    void AddCustomAfterUpdateFunction(BasicFunctionWrapper callBack) {
        AfterUpdateFunctionWrapper.emplace_back(std::move(callBack));
    }

    glm::vec2 gravity = {0, 9.81f * TGForceFactor};

    FPSCounter fpsc;

  private:
    PhysicsSystem() : running(false) { ; }
    ~PhysicsSystem() = default;
    PhysicsSystem(const PhysicsSystem &) = delete;
    PhysicsSystem &operator=(const PhysicsSystem &) = delete;
    void HandleSDLEvents(SDL_Event &event);
    void CollisionHandler();
    bool
    Collision(RigidBody *a, RigidBody *b, glm::vec2 &norm,
              float &depth); // this is only should used by CollisionHandler()
    void CollisionResolver(RigidBody *a, RigidBody *b, glm::vec2 norm,
                           float depth); // so does this.

    void OutOffBoundCheck();

    bool running;
    bool hasRemoveCalled = false;
    int iteration_;

    EventHandlerID eventHandler_1;
    EventHandlerID eventHandler_2;
    EventHandlerID eventHandler_3;
    EventHandlerID eventHandler_4;

    std::shared_ptr<ObjectWorld> rootNode;
    std::vector<std::shared_ptr<ObjectWorld>> physicsObjectsToAdd;

    std::vector<BasicFunctionWrapper> initFunctionWrapper;
    std::vector<BasicFunctionWrapper> AfterUpdateFunctionWrapper;
};

#define GET_PhysicsSystem PhysicsSystem::Instance()