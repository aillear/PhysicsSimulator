# pragma once

#include <glm/ext/vector_float2.hpp>
enum class PhysicsShapeType {
    CIRCLE = 0,
    BOX,
    POLYGON
};

struct AABB {
    glm::vec2 minP;
    glm::vec2 maxP;
};