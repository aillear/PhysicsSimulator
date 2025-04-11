# include "collisionMgr.h"
#include <_mingw_stat64.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>

CollisionMgr& CollisionMgr::Instance() {
    static CollisionMgr instance;
    return instance;
} 

void CollisionMgr::Init() {

}

void CollisionMgr::Destory() {

}

bool CollisionMgr::IntersectCircle(GlmCircle a, GlmCircle b, glm::vec2& norm, float& depth) {
    float distanceCenter = glm::distance(a.center, b.center);
    float distanceSafe = a.radius + b.radius;
    norm = {0, 0};
    depth = 0;

    if (distanceCenter >= distanceSafe) return false;

    norm = glm::normalize(b.center - a.center);
    depth = distanceSafe - distanceCenter;

    return true;
}