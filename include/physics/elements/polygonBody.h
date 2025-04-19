#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "material.h"
#include "rigidbody.h"
#include <glm/ext/vector_float2.hpp>
#include <vector>
class PolygonBody : public RigidBody {
  public:
  /**
   * @brief Construct a new Polygon Body object
   * 
   * @param mate 
   * @param vertices vertices of the polygon, should be in counter-clockwise order
   * @note the mass center of the polygon will be calculated based on these vertices.
   */
    PolygonBody(Material mate, std::vector<glm::vec2>& vertices);

    void SetColor(SDL_Color color) override;
    void SetFColor(SDL_FColor color) override;

    void SetColorBoundry(SDL_Color color) override;
    void SetFColorBoundry(SDL_FColor color) override;

    const std::vector<glm::vec2>& GetVertices() override;
    const AABB& GetAABB() override;

  protected:
    void Render() override;
    void CalRotateInertia() override;


    std::vector<glm::vec2> OriginVertices;
    std::vector<glm::vec2> TransformedVertices;
    std::vector<SDL_Vertex> TransformedVerticesF;
    std::vector<SDL_Vertex> TransformedVerticesB;

    private:
    void GetArea(); // get area when construct;
    void GetCentroidPosition(); // get mass center position when construct;
};