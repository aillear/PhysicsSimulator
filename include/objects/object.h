#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_events.h>
#include <cstddef>
#include <functional>
#include <glm/ext/vector_float2.hpp>
#include <string>
#include <vector>

/**
 * @brief base class for all objects in the game.
 *    provide basic init, update, render, destroy virtual functions.
 *    provide unique id for each object automatically.
 *
 */

using BasicFunctionWrapper = std::function<void()>;
using UpdateFunctionWrapper = std::function<void(float)>;
using EventFunctionWrapper = std::function<void(SDL_Event&)>;

using ObjectID = Uint64;
class Object {
  public:
    Object() : objectID(objectCount++) { ; };
    virtual ~Object() = default;
    // life cycle wrapper, systems can manage object's lifecycle by this
    void InitWrapper();
    void RenderWrapper();
    void UpdateWrapper(float dt);
    void PhysicsUpdateWrapper(float dt);
    void HandleEventWrapper(SDL_Event &event);
    void DestroyWrapper();

    void SetToRemove() {removedMark = true;}
    void CheckChildToRemove();

    // event handle interface
    virtual void OnMouseMove(SDL_Event &event) = 0;
    virtual void OnMouseDown(SDL_Event &event) = 0;
    virtual void OnMouseUp(SDL_Event &event) = 0;

    bool GetEnabled() const { return enabled; }
    void SetEnabled(bool enabled) { this->enabled = enabled; }
    void SetName(const std::string &name) { this->name = name; }
    const std::string &GetName() const { return name; }
    const ObjectID GetID() const { return objectID; }

    // child management
    void AddChild(std::shared_ptr<Object> child);
    // remove first child by pointer, id or name
    // do not recursively remove child
    // remove child my be reconscruct in the future.
    void RemoveChild(std::shared_ptr<Object> child);
    void RemoveChlidByID(ObjectID id);
    void RemoveChlidByName(const std::string &name);
    void RemoveAllChildren();

    // get first child by id or name, if not found, return nullptr
    // it will recursively search all children
    std::shared_ptr<Object> GetChildByID(ObjectID id);
    std::shared_ptr<Object> GetChildByName(const std::string &name);

    std::vector<std::shared_ptr<Object>> GetChildren() const {return children;}

    Object *GetParent() const { return parent; }
    void SetParent(Object *parent) { this->parent = parent; }


  protected:

    // live cycle interface
    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void Update(float dt) = 0;
    virtual void PhysicsUpdate(float dt) = 0;
    virtual void HandleEvent(SDL_Event &event) = 0;
    virtual void Destroy() = 0;

    // you can add addition logic to update here
    void AddInitCallBack(BasicFunctionWrapper callBack);
    void AddRenderCallBack(BasicFunctionWrapper callBack);
    void AddUpdateCallBack(UpdateFunctionWrapper callBack);
    void AddPhysicsUpdateCallBack(UpdateFunctionWrapper callBack);
    void AddHandleEventCallBack(EventFunctionWrapper callBack);
    void AddDestroyCallBack(BasicFunctionWrapper callBack);


    bool enabled = true;
    bool removedMark = false;
    std::string name;

    ObjectID objectID;
    static ObjectID objectCount; // I believe that size_t is enough for the number of objects in the game
    Object *parent = nullptr;
    std::vector<std::shared_ptr<Object>> children;

  private:
    // release all children and itself recursively.
    void ReleaseAllChildren();
    // call back, so that child can easily add new logic to life cycle.
    std::vector<BasicFunctionWrapper> initCallBacks;
    std::vector<BasicFunctionWrapper> renderCallBacks;
    std::vector<UpdateFunctionWrapper> updateCallBacks;
    std::vector<UpdateFunctionWrapper> physicsUpdateCallBacks;
    std::vector<EventFunctionWrapper> handleEventCallBacks;
    std::vector<BasicFunctionWrapper> destroyCallBacks;
};