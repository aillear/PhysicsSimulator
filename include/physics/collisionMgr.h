# pragma once   

#include "renderSystem.h"
#include <glm/ext/vector_float2.hpp>
class CollisionMgr {
  public:
    static CollisionMgr& Instance();

    void Init();
    void Destory();

    bool IntersectCircle(GlmCircle a, GlmCircle b, glm::vec2& norm, float& depth);
  private:
    
    CollisionMgr() = default;
    ~CollisionMgr() = default; 
};


# define GET_CollisionMgr CollisionMgr::Instance()