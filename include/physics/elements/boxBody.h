#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "material.h"
#include "rigidbody.h"
#include "transform.h"
#include <array>
#include <glm/ext/vector_float2.hpp>
#include <vector>
class BoxBody : public RigidBody {
  public:
    BoxBody(Material mate, glm::vec2 position, glm::vec2 widthHeight);

    glm::vec2 GetWidthHeight() const override { return widthHeight_; }
    void SetWidthHeight(glm::vec2 wh) override { widthHeight_ = wh; }

    void Move(glm::vec2 ds) override;
    void MoveTo(glm::vec2 destinaion) override;
    void Rotate(float angles) override;
    void RotateTo(float angles) override;


    void SetPosition(const glm::vec2& position) override;
    void SetPosition(float x, float y) override;

    void SetColor(SDL_Color color) override;
    void SetFColor(SDL_FColor color) override;

    void SetColorBoundry(SDL_Color color) override;
    void SetFColorBoundry(SDL_FColor color) override;

    const std::vector<SDL_Vertex>& GetVertex() const override {return TransformedVertexB;}

  protected:
    void Render() override;
    void PhysicsUpdate(float dt) override;

    void GetVertexTransfrom() override;
    void GetAABBUpdated() override;
    void CalRotateIntertia() override;

    glm::vec2 widthHeight_;
    Transform2D transformer;
    std::array<glm::vec2, 4> OriginVertex;
    std::vector<SDL_Vertex> TransformedVertexF;
    std::vector<SDL_Vertex> TransformedVertexB;
};