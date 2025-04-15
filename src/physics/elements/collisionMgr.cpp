#include "collisionMgr.h"
#include "conversion.h"
#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <limits>
#include <utility>

CollisionMgr &CollisionMgr::Instance() {
    static CollisionMgr instance;
    return instance;
}

void CollisionMgr::Init() {}

void CollisionMgr::Destroy() {}

bool CollisionMgr::CollisionCheck(RigidBody *a, RigidBody *b, glm::vec2 &norm,
                                  float &depth) {
    using Shape = PhysicsShapeType;
    norm = {0, 0};
    depth = 0;

    Shape typeA = a->GetPhysicsType();
    Shape typeB = b->GetPhysicsType();

    if (typeA == Shape::CIRCLE) {
        if (typeB == Shape::CIRCLE) {
            return GET_CollisionMgr.IntersectCircle(
                a->GetCircle(), b->GetCircle(), norm, depth);
        } else {
            return GET_CollisionMgr.IntersectPolygonAndCircle(
                a->GetCircle(), b->GetVertex(), b->GetPosition(), norm, depth);
        }
    } else {
        if (typeB == Shape::CIRCLE) {
            bool result = GET_CollisionMgr.IntersectPolygonAndCircle(
                b->GetCircle(), a->GetVertex(), a->GetPosition(), norm, depth);
            norm = -norm;
            return result;
        } else {
            return GET_CollisionMgr.IntersectPolygon(
                a->GetVertex(), a->GetPosition(), b->GetVertex(),
                b->GetPosition(), norm, depth);
        }
    }
}

void CollisionMgr::FindContactPoints(RigidBody *a, RigidBody *b,
                                     glm::vec2 &contactP1, glm::vec2 &contactP2,
                                     int &count) {
    using Shape = PhysicsShapeType;
    contactP1 = {0, 0};
    contactP2 = {0, 0};
    count = 0;

    Shape typeA = a->GetPhysicsType();
    Shape typeB = b->GetPhysicsType();

    if (typeA == Shape::CIRCLE) {
        if (typeB == Shape::CIRCLE) {
            FindContactPoints(a->GetCircle(), b->GetCircle(), contactP1);
            count = 1;
        } else {
        }
    } else {
        if (typeB == Shape::CIRCLE) {

        } else {
        }
    }
}

void CollisionMgr::FindContactPoints(GlmCircle a, GlmCircle b,
                                     glm::vec2 &contactPoint) {
    contactPoint = a.center + a.radius * glm::normalize((b.center - a.center));
}

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
bool CollisionMgr::IntersectPolygon(const std::vector<SDL_Vertex> &a,
                                    const std::vector<SDL_Vertex> &b,
                                    glm::vec2 &norm, float &depth) {

    norm = {0, 0};
    depth = std::numeric_limits<float>::max();

    for (int i = 0; i < a.size(); i++) {
        auto va = a[i].position;
        auto edge = a[(i + 1) % a.size()].position - va;

        glm::vec2 axis = glm::normalize(glm::vec2{-edge.y, edge.x});
        glm::vec2 pa = GetProject(a, axis);
        glm::vec2 pb = GetProject(b, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;

        float axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
        if (axisDepth < depth) {
            depth = axisDepth;
            norm = axis;
        }
    }

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i].position;
        auto edge = b[(i + 1) % b.size()].position - va;

        glm::vec2 axis = glm::normalize(glm::vec2{-edge.y, edge.x});
        glm::vec2 pa = GetProject(a, axis);
        glm::vec2 pb = GetProject(b, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;
        float axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
        if (axisDepth < depth) {
            depth = axisDepth;
            norm = axis;
        }
    }
    auto direction = GetArithmeticMean(b) - GetArithmeticMean(a);
    if (glm::dot(direction, norm) < 0.0f)
        norm = -norm;
    return true;
}

bool CollisionMgr::IntersectPolygon(const std::vector<SDL_Vertex> &a,
                                    const glm::vec2 &centerA,
                                    const std::vector<SDL_Vertex> &b,
                                    const glm::vec2 &centerB, glm::vec2 &norm,
                                    float &depth) {
    norm = {0, 0};
    depth = std::numeric_limits<float>::max();

    for (int i = 0; i < a.size(); i++) {
        auto va = a[i].position;
        auto edge = a[(i + 1) % a.size()].position - va;

        glm::vec2 axis = glm::normalize(glm::vec2{-edge.y, edge.x});
        glm::vec2 pa = GetProject(a, axis);
        glm::vec2 pb = GetProject(b, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;

        float axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
        if (axisDepth < depth) {
            depth = axisDepth;
            norm = axis;
        }
    }

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i].position;
        auto edge = b[(i + 1) % b.size()].position - va;

        glm::vec2 axis = glm::normalize(glm::vec2{-edge.y, edge.x});
        glm::vec2 pa = GetProject(a, axis);
        glm::vec2 pb = GetProject(b, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;
        float axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
        if (axisDepth < depth) {
            depth = axisDepth;
            norm = axis;
        }
    }
    auto direction = centerB - centerA;
    if (glm::dot(direction, norm) < 0.0f)
        norm = -norm;
    return true;
}

bool CollisionMgr::IntersectPolygonAndCircle(const GlmCircle &a,
                                             const std::vector<SDL_Vertex> &b,
                                             glm::vec2 &norm, float &depth) {
    norm = {0, 0};
    depth = std::numeric_limits<float>::max();
    glm::vec2 axis;
    float axisDepth;
    glm::vec2 pa, pb;

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i].position;
        auto edge = b[(i + 1) % b.size()].position - va;

        axis = glm::normalize(glm::vec2{-edge.y, edge.x});
        pa = GetProject(b, axis);
        pb = GetProjectCircle(a, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;

        axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
        if (axisDepth < depth) {
            depth = axisDepth;
            norm = axis;
        }
    }

    glm::vec2 closePoint =
        ToGlmVec2(b[GetClosestPointIndexToCircle(a, b)].position);
    axis = glm::normalize(closePoint - a.center);
    pa = GetProject(b, axis);
    pb = GetProjectCircle(a, axis);
    if (pa.x >= pb.y || pb.x >= pa.y)
        return false;

    axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
    if (axisDepth < depth) {
        depth = axisDepth;
        norm = axis;
    }

    auto direction = GetArithmeticMean(b) - a.center;
    if (glm::dot(direction, norm) < 0.0f)
        norm = -norm;

    return true;
}

bool CollisionMgr::IntersectPolygonAndCircle(const GlmCircle &a,
                                             const std::vector<SDL_Vertex> &b,
                                             const glm::vec2 &centerB,
                                             glm::vec2 &norm, float &depth) {
    norm = {0, 0};
    depth = std::numeric_limits<float>::max();
    glm::vec2 axis;
    float axisDepth;
    glm::vec2 pa, pb;

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i].position;
        auto edge = b[(i + 1) % b.size()].position - va;

        axis = glm::normalize(glm::vec2{-edge.y, edge.x});
        pa = GetProject(b, axis);
        pb = GetProjectCircle(a, axis);
        if (pa.x >= pb.y || pb.x >= pa.y)
            return false;

        axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
        if (axisDepth < depth) {
            depth = axisDepth;
            norm = axis;
        }
    }

    glm::vec2 closePoint =
        ToGlmVec2(b[GetClosestPointIndexToCircle(a, b)].position);
    axis = glm::normalize(closePoint - a.center);
    pa = GetProject(b, axis);
    pb = GetProjectCircle(a, axis);
    if (pa.x >= pb.y || pb.x >= pa.y)
        return false;

    axisDepth = std::min(pa.y - pb.x, pb.y - pa.x);
    if (axisDepth < depth) {
        depth = axisDepth;
        norm = axis;
    }

    auto direction = centerB - a.center;
    if (glm::dot(direction, norm) < 0.0f)
        norm = -norm;

    return true;
}

glm::vec2 CollisionMgr::GetProjectCircle(const GlmCircle &circle,
                                         glm::vec2 axis) {
    glm::vec2 res = {std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::min()};
    glm::vec2 directR = glm::normalize(axis) * circle.radius;

    res.x = glm::dot(circle.center + directR, axis);
    res.y = glm::dot(circle.center - directR, axis);
    if (res.x > res.y)
        std::swap(res.x, res.y);

    return res;
}

glm::vec2 CollisionMgr::GetProject(const std::vector<SDL_Vertex> &vertices,
                                   glm::vec2 axis) {
    glm::vec2 res = {std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::min()};

    for (auto &vertex : vertices) {
        glm::vec2 v = ToGlmVec2(vertex.position);
        float proj = glm::dot(v, axis);

        res.x = std::min(res.x, proj);
        res.y = std::max(res.y, proj);
    }
    return res;
}

glm::vec2
CollisionMgr::GetArithmeticMean(const std::vector<SDL_Vertex> &vertices) {
    glm::vec2 center = {0, 0};
    for (auto &vertex : vertices) {
        center.x += vertex.position.x;
        center.y += vertex.position.y;
    }
    int size = vertices.size();
    return center / (float)size;
}

int CollisionMgr::GetClosestPointIndexToCircle(
    const GlmCircle &circle, const std::vector<SDL_Vertex> &vertices) {
    int result = 0;
    float minD = glm::distance(circle.center, ToGlmVec2(vertices[0].position));
    for (int i = 1; i < vertices.size(); i++) {
        float dist =
            glm::distance(circle.center, ToGlmVec2(vertices[i].position));
        if (dist < minD) {
            dist = minD;
            result = i;
        }
    }
    return result;
}