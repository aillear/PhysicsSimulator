# pragma once

#include "logger.h"
#include "physicsSystem.h"
#include "exceptions.h"
struct Material {
    float density;
    float resilience;
    float frictionFactor;

    Material(float density, float resilience, float frictionFactor) : density(density), resilience(resilience), frictionFactor(frictionFactor) {
        if (density < GET_PhysicsSystem.MinDensity) {
            F_LOG_ERROR("RigidBody density {} is too small, it must be bigger than {}", density, GET_PhysicsSystem.MinDensity);
            throw MaterialDensityError(density);
        }
        if (density > GET_PhysicsSystem.MaxDensity) {
            F_LOG_ERROR("RigidBody density {} is too big, it must be smaller than {}", density, GET_PhysicsSystem.MaxDensity);
            throw MaterialDensityError(density);
        }
    }
};