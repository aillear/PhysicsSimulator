# pragma once

#include <glm/ext/vector_float2.hpp>

// TODO: add more physics properties
// may define a struct call material.
class RigidBody {
    glm::vec2 position;
    glm::vec2 velocity;

    float rotation;
    float angularVelocity;

    float mass; 
    float density;
    float area;
    float resilience;

    bool isStatic;

};