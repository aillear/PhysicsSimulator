# pragma once

#include "SDL3/SDL_events.h"

/**
 * @brief base class for all objects in the game.
 * 
 */
class Object {
    public:
        Object() = default;
        virtual ~Object() = default;
        virtual void Init() = 0;
        virtual void Render() = 0;
        virtual void Update(float dt) = 0;
        virtual void PhysicsUpdate(float dt) = 0;
        virtual void HandleEvent(SDL_Event &event) = 0;
        virtual void Destroy() = 0;
    private:
        int selfEventID;
        int updateID;
};