#include "collisionMgr.h"
#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>
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
            FindContactPoints(a->GetCircle(), b->GetVertex(), b->GetPosition(),
                              contactP1);
            count = 1;
        }
    } else {
        if (typeB == Shape::CIRCLE) {
            FindContactPoints(b->GetCircle(), a->GetVertex(), a->GetPosition(),
                              contactP1);
            count = 1;
        } else {
            FindContactPoints(a->GetVertex(), b->GetVertex(), contactP1,
                              contactP2, count);
        }
    }
}

void CollisionMgr::PointSegmentDistance(const glm::vec2 &p, const glm::vec2 &a,
                                        const glm::vec2 &b,
                                        float &distanceSquired, glm::vec2 &cp) {
    glm::vec2 ab = b - a;
    glm::vec2 ap = p - a;

    float proj = glm::dot(ap, ab);
    float abLenSq = glm::dot(ab, ab);
    float d = proj / abLenSq;

    if (d <= 0.0f)
        cp = a;
    else if (d >= 1.0f)
        cp = b;
    else
        cp = a + d * ab;

    // ab is used to calculate the distance from point to segment.
    // not it's origin useage.
    ab = p - cp;
    distanceSquired = glm::dot(ab, ab);
}

void CollisionMgr::FindContactPoints(const GlmCircle &a, const GlmCircle &b,
                                     glm::vec2 &contactPoint) {
    contactPoint = a.center + a.radius * glm::normalize((b.center - a.center));
}

void CollisionMgr::FindContactPoints(const GlmCircle &a,
                                     const std::vector<glm::vec2> &b,
                                     const glm::vec2 &centerB, glm::vec2 &cp) {
    cp = {0, 0};
    float minD = std::numeric_limits<float>::max();
    float distSq;
    glm::vec2 contact;

    glm::vec2 va = b.back();
    glm::vec2 vb = b[0];
    PointSegmentDistance(a.center, va, vb, distSq, contact);
    if (distSq < minD) {
        minD = distSq;
        cp = contact;
    }

    for (int i = 1; i < b.size(); i++) {

        va = b[i - 1];
        vb = b[i];

        PointSegmentDistance(a.center, va, vb, distSq, contact);
        if (distSq < minD) {
            minD = distSq;
            cp = contact;
        }
    }
}

void CollisionMgr::FindContactPoints(const std::vector<glm::vec2> &a,
                                     const std::vector<glm::vec2> &b,
                                     glm::vec2 &contactPoint1,
                                     glm::vec2 &contactPoint2, int &count) {

    contactPoint1 = {0, 0};
    contactPoint2 = {0, 0};
    count = 0;

    float minD = std::numeric_limits<float>::max();
    float distSq;
    glm::vec2 cp;

    for (int i = 0; i < a.size(); i++) {
        glm::vec2 p = a[i];

        glm::vec2 va = b.back();
        glm::vec2 vb = b[0];

        // pre step
        {
            PointSegmentDistance(p, va, vb, distSq, cp);
            if (glm::epsilonEqual(distSq, minD, 0.0001f)) {
                if (!glm::all(glm::epsilonEqual(cp, contactPoint1, 0.0001f))) {
                    contactPoint2 = cp;
                    count = 2;
                }
            } else if (distSq < minD) {
                minD = distSq;
                contactPoint1 = cp;
                count = 1;
            }
        }
        for (int j = 1; j < b.size(); j++) {
            glm::vec2 va = b[j - 1];
            glm::vec2 vb = b[j];

            PointSegmentDistance(p, va, vb, distSq, cp);
            if (glm::epsilonEqual(distSq, minD, 0.0001f)) {
                if (!glm::all(glm::epsilonEqual(cp, contactPoint1, 0.0001f))) {
                    contactPoint2 = cp;
                    count = 2;
                }
            } else if (distSq < minD) {
                minD = distSq;
                contactPoint1 = cp;
                count = 1;
            }
        }
    }

    for (int i = 0; i < b.size(); i++) {
        glm::vec2 p = b[i];

        glm::vec2 va = a.back();
        glm::vec2 vb = a[0];

        // pre step
        {
            PointSegmentDistance(p, va, vb, distSq, cp);
            if (glm::epsilonEqual(distSq, minD, 0.0001f)) {
                if (!glm::all(glm::epsilonEqual(cp, contactPoint1, 0.0001f))) {
                    contactPoint2 = cp;
                    count = 2;
                }
            } else if (distSq < minD) {
                minD = distSq;
                contactPoint1 = cp;
                count = 1;
            }
        }
        for (int j = 1; j < a.size(); j++) {
            glm::vec2 va = a[j - 1];
            glm::vec2 vb = a[j];

            PointSegmentDistance(p, va, vb, distSq, cp);
            if (glm::epsilonEqual(distSq, minD, 0.0001f)) {
                if (!glm::all(glm::epsilonEqual(cp, contactPoint1, 0.0001f))) {
                    contactPoint2 = cp;
                    count = 2;
                }
            } else if (distSq < minD) {
                minD = distSq;
                contactPoint1 = cp;
                count = 1;
            }
        }
    }
}

bool CollisionMgr::IntersectCircle(const GlmCircle &a, const GlmCircle &b,
                                   glm::vec2 &norm, float &depth) {
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
bool CollisionMgr::IntersectPolygon(const std::vector<glm::vec2> &a,
                                    const std::vector<glm::vec2> &b,
                                    glm::vec2 &norm, float &depth) {

    norm = {0, 0};
    depth = std::numeric_limits<float>::max();

    for (int i = 0; i < a.size(); i++) {
        auto va = a[i];
        auto edge = a[(i + 1) % a.size()] - va;

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
        auto va = b[i];
        auto edge = b[(i + 1) % b.size()] - va;

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

bool CollisionMgr::IntersectPolygon(const std::vector<glm::vec2> &a,
                                    const glm::vec2 &centerA,
                                    const std::vector<glm::vec2> &b,
                                    const glm::vec2 &centerB, glm::vec2 &norm,
                                    float &depth) {
    norm = {0, 0};
    depth = std::numeric_limits<float>::max();

    for (int i = 0; i < a.size(); i++) {
        auto va = a[i];
        auto edge = a[(i + 1) % a.size()] - va;

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
        auto va = b[i];
        auto edge = b[(i + 1) % b.size()] - va;

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
                                             const std::vector<glm::vec2> &b,
                                             glm::vec2 &norm, float &depth) {
    norm = {0, 0};
    depth = std::numeric_limits<float>::max();
    glm::vec2 axis;
    float axisDepth;
    glm::vec2 pa, pb;

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i];
        auto edge = b[(i + 1) % b.size()] - va;

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

    glm::vec2 closePoint = b[GetClosestPointIndexToCircle(a, b)];
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
                                             const std::vector<glm::vec2> &b,
                                             const glm::vec2 &centerB,
                                             glm::vec2 &norm, float &depth) {
    norm = {0, 0};
    depth = std::numeric_limits<float>::max();
    glm::vec2 axis;
    float axisDepth;
    glm::vec2 pa, pb;

    for (int i = 0; i < b.size(); i++) {
        auto va = b[i];
        auto edge = b[(i + 1) % b.size()] - va;

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

    glm::vec2 closePoint = b[GetClosestPointIndexToCircle(a, b)];
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

glm::vec2 CollisionMgr::GetProject(const std::vector<glm::vec2> &vertices,
                                   glm::vec2 axis) {
    glm::vec2 res = {std::numeric_limits<float>::max(),
                     std::numeric_limits<float>::min()};

    for (auto &vertex : vertices) {
        glm::vec2 v = vertex;
        float proj = glm::dot(v, axis);

        res.x = std::min(res.x, proj);
        res.y = std::max(res.y, proj);
    }
    return res;
}

glm::vec2
CollisionMgr::GetArithmeticMean(const std::vector<glm::vec2> &vertices) {
    glm::vec2 center = {0, 0};
    for (auto &vertex : vertices) {
        center.x += vertex.x;
        center.y += vertex.y;
    }
    int size = vertices.size();
    return center / (float)size;
}

int CollisionMgr::GetClosestPointIndexToCircle(
    const GlmCircle &circle, const std::vector<glm::vec2> &vertices) {
    int result = 0;
    float minD = glm::distance(circle.center, vertices[0]);
    for (int i = 1; i < vertices.size(); i++) {
        float dist = glm::distance(circle.center, vertices[i]);
        if (dist < minD) {
            dist = minD;
            result = i;
        }
    }
    return result;
}

bool CollisionMgr::IntersectAABBs(const AABB &a, const AABB &b) {
    if (a.maxP.x <= b.minP.x || a.minP.x >= b.maxP.x)
        return false;
    if (a.maxP.y <= b.minP.y || a.minP.y >= b.maxP.y)
        return false;

    return true;
}