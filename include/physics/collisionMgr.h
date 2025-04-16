#pragma once

#include "SDL3/SDL_render.h"
#include "renderSystem.h"
#include "rigidbody.h"
#include "shape.h"
#include <glm/ext/vector_float2.hpp>
#include <vector>

struct Collision {
    RigidBody *objA;
    RigidBody *objB;
    glm::vec2 norm;
    float depth;
    glm::vec2 point1;
    glm::vec2 point2;
    int collisionCount;
};

class CollisionMgr {
  public:
    static CollisionMgr &Instance();

    void Init();
    void Destroy();
    bool CollisionCheck(RigidBody *a, RigidBody *b, glm::vec2 &norm,
                        float &depth);

    void FindContactPoints(RigidBody *a, RigidBody *b, glm::vec2 &contactP1,
                           glm::vec2 &contactP2, int &count);

    bool IntersectCircle(const GlmCircle &a, const GlmCircle &b,
                         glm::vec2 &norm, float &depth);
    bool IntersectPolygon(const std::vector<SDL_Vertex> &a,
                          const std::vector<SDL_Vertex> &b, glm::vec2 &norm,
                          float &depth);
    bool IntersectPolygon(const std::vector<SDL_Vertex> &a,
                          const glm::vec2 &centerA,
                          const std::vector<SDL_Vertex> &b,
                          const glm::vec2 &centerB, glm::vec2 &norm,
                          float &depth);
    bool IntersectPolygonAndCircle(const GlmCircle &a,
                                   const std::vector<SDL_Vertex> &b,
                                   glm::vec2 &norm, float &depth);

    bool IntersectPolygonAndCircle(const GlmCircle &a,
                                   const std::vector<SDL_Vertex> &b,
                                   const glm::vec2 &centerB, glm::vec2 &norm,
                                   float &depth);

    bool IntersectAABBs(const AABB &a, const AABB &b);

    void PointSegmentDistance(const glm::vec2 &p, const glm::vec2 &a,
                              const glm::vec2 &b, float &distanceSquired,
                              glm::vec2 &cp);

  private:
    void FindContactPoints(const GlmCircle &a, const GlmCircle &b,
                           glm::vec2 &contactPoint);
    void FindContactPoints(const GlmCircle &a, const std::vector<SDL_Vertex> &b,
                           const glm::vec2 &centerB, glm::vec2 &contactPoint);
    void FindContactPoints(const std::vector<SDL_Vertex> &a,
                           const std::vector<SDL_Vertex> &b,
                           glm::vec2 &contactPoint1, glm::vec2 &contactPoint2,
                           int &count);
    glm::vec2 GetProject(const std::vector<SDL_Vertex> &vertices,
                         glm::vec2 axis);
    glm::vec2 GetProjectCircle(const GlmCircle &circle, glm::vec2 axis);
    glm::vec2 GetArithmeticMean(const std::vector<SDL_Vertex> &vertices);
    int GetClosestPointIndexToCircle(const GlmCircle &circle,
                                     const std::vector<SDL_Vertex> &vertices);

    CollisionMgr() = default;
    ~CollisionMgr() = default;
};

#define GET_CollisionMgr CollisionMgr::Instance()