# pragma once   

#include "SDL3/SDL_render.h"
#include "renderSystem.h"
#include <glm/ext/vector_float2.hpp>
#include <vector>
class CollisionMgr {
  public:
    static CollisionMgr& Instance();

    void Init();
    void Destory();

    bool IntersectCircle(GlmCircle a, GlmCircle b, glm::vec2& norm, float& depth);
    bool IntersectPolygon(const std::vector<SDL_Vertex>& a, const std::vector<SDL_Vertex>& b, glm::vec2& norm, float& depth);
    bool IntersectPolygonAndCircle(const GlmCircle& a, const std::vector<SDL_Vertex>& b, glm::vec2& norm, float& depth);
  
  private:

    glm::vec2 GetProject(const std::vector<SDL_Vertex>& vertices, glm::vec2 axis);
    glm::vec2 GetProjectCircle(const GlmCircle& circle, glm::vec2 axis);
    glm::vec2 GetArithmeticMean(const std::vector<SDL_Vertex>& vertices);
    int GetClosestPointIndexToCircle(const GlmCircle& circle, const std::vector<SDL_Vertex>& vertices);
    
    CollisionMgr() = default;
    ~CollisionMgr() = default; 
};


# define GET_CollisionMgr CollisionMgr::Instance()