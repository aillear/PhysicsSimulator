# pragma once
#include "vector2.h"

class RigidBody {
    Vector2f position;
    Vector2f velocity;

    float rotation;
    float angularVelocity;

    float mass; 
    float density;
    float area;
    float resilience;

    bool isStatic;

};