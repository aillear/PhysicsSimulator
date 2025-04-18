# pragma once

struct Material {
    float density;
    float resilience;
    float StaticfrictionFactor;
    float DynamicfrictionFactor;

    Material(float density, float resilience, float StaticfrictionFactor, float DynamicfrictionFactor);
};