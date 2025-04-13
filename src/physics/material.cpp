#include "material.h"
#include "exceptions.h"
#include "logger.h"
#include "configs.h"

Material::Material(float density, float resilience, float frictionFactor) : density(density), resilience(resilience), frictionFactor(frictionFactor) {
    if (density < MinDensity) {
        F_LOG_ERROR("RigidBody density {} is too small, it must be bigger than {}", density, MinDensity);
        throw MaterialDensityError(density);
    }
    if (density > MaxDensity) {
        F_LOG_ERROR("RigidBody density {} is too big, it must be smaller than {}", density, MaxDensity);
        throw MaterialDensityError(density);
    }
}