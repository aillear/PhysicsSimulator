#include "collisionMgr.h"
#include "conversion.h"
#include "inputSystem.h"
#include <_mingw_stat64.h>
#include <algorithm>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <limits>

CollisionMgr &CollisionMgr::Instance() {
    static CollisionMgr instance;
    return instance;
}

void CollisionMgr::Init() {}

void CollisionMgr::Destory() {}

bool CollisionMgr::IntersectCircle(GlmCircle a, GlmCircle b, glm::vec2 &norm,
                                   float &depth) {
    float distanceCenter = glm::distance(a.center, b.center);
    float distanceSafe = a.radius + b.radius;
    norm = {0, 0};
    depth = 0;

    if (distanceCenter >= distanceSafe)
        return false;

    norm = glm::normalize(b.center - a.center);
    depth = distanceSafe - distanceCenter;

    return true;
}

// SAT collision detect.
bool CollisionMgr::IntersectPolygon(const std::vector<SDL_Vertex>& a,
                                    const std::vector<SDL_Vertex>& b) {
    for (int i = 0; i < a.size(); i++) {
        auto va = a[i].position;
        auto edge = a[(i + 1) % a.size()].position - va;

        glm::vec2 axis = {-edge.y, edge.x};
        glm::vec2 pa = GetProject(a, axis);
        glm::vec2 pb = GetProject(b, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;
    }

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i].position;
        auto edge = b[(i + 1) % b.size()].position - va;

        glm::vec2 axis = {-edge.y, edge.x};
        glm::vec2 pa = GetProject(a, axis);
        glm::vec2 pb = GetProject(b, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;
    }
    return true;
}

glm::vec2 CollisionMgr::GetProject(const std::vector<SDL_Vertex>& vertexs,
                                   glm::vec2 axis) {
    glm::vec2 res = {std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::min()};

    for (auto &vertex : vertexs) {
        glm::vec2 v = ToGlmVec2(vertex.position);
        float proj = glm::dot(v, axis);

        res.x = std::min(res.x, proj);
        res.y = std::max(res.y, proj);
    }
    return res;
}