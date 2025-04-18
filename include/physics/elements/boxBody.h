#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "material.h"
#include "rigidbody.h"
#include <array>
#include <glm/ext/vector_float2.hpp>
#include <vector>
class BoxBody : public RigidBody {
  public:
    BoxBody(Material mate, glm::vec2 widthHeight);

    glm::vec2 GetWidthHeight() const override { return widthHeight_; }
    void SetWidthHeight(glm::vec2 wh) override { widthHeight_ = wh; }

    void SetColor(SDL_Color color) override;
    void SetFColor(SDL_FColor color) override;

    void SetColorBoundry(SDL_Color color) override;
    void SetFColorBoundry(SDL_FColor color) override;

    const std::vector<glm::vec2>& GetVertices() override;
    const AABB& GetAABB() override;

  protected:
    void Render() override;
    void PhysicsUpdate(float dt) override;


    void CalRotateIntertia() override;

    glm::vec2 widthHeight_;
    std::array<glm::vec2, 4> OriginVertex;
    std::vector<glm::vec2> TransformedVertex;
    std::vector<SDL_Vertex> TransformedVertexF;
    std::vector<SDL_Vertex> TransformedVertexB;
};