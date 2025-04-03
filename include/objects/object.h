#pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"
#include <glm/ext/vector_float2.hpp>
#include <string>
#include <vector>

/**
 * @brief base class for all objects in the game.
 *    provide basic init, update, render, destroy virtual functions.
 *    provide unique id for each object automatically.
 *
 */
class Object {
  public:
    Object() : objectID(objectCount++) { ; };
    virtual ~Object() = default;
    void UpdateWrapper(float dt);
    void PhysicsUpdateWrapper(float dt);
    void RenderWrapper();
    void HandleEventWrapper(SDL_Event &event);
    // live cycle interface
    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void Update(float dt) = 0;
    virtual void PhysicsUpdate(float dt) = 0;
    virtual void HandleEvent(SDL_Event &event) = 0;
    virtual void Destroy() = 0;

    // event handle interface
    virtual void OnMouseMove(SDL_Event &event) = 0;
    virtual void OnMouseDown(SDL_Event &event) = 0;
    virtual void OnMouseUp(SDL_Event &event) = 0;

    bool GetEnabled() const { return enabled; }
    void SetEnabled(bool enabled) { this->enabled = enabled; }
    void SetName(const std::string &name) { this->name = name; }
    const std::string &GetName() const { return name; }
    Uint32 GetID() const { return objectID; }

    // child management
    void AddChild(std::shared_ptr<Object> child);
    // remove first child by pointer, id or name
    // do not recursively remove child
    void RemoveChild(std::shared_ptr<Object> child);
    void RemoveChlidByID(Uint32 id);
    void RemoveChlidByName(const std::string &name);
    void ClearChildren();

    // get first child by id or name, if not found, return nullptr
    // it will recursively search all children
    std::shared_ptr<Object> GetChildByID(Uint32 id);
    std::shared_ptr<Object> GetChildByName(const std::string &name);

    std::vector<std::shared_ptr<Object>> GetChildren() const {return children;}

    Object *GetParent() const { return parent; }

    void SetParent(Object *parent) { this->parent = parent; }

    

  protected:
    bool enabled = true;
    int selfEventID;
    int updateID;
    std::string name;

    Uint32 objectID;
    static Uint32 objectCount; // I believe that Uint32_t is enough for the number of objects in the game
    Object *parent = nullptr;
    std::vector<std::shared_ptr<Object>> children;
};