#include "polygonBody.h"
#include "SDL3/SDL_render.h"
#include "configs.h"
#include "conversion.h"
#include "renderBufferMgr.h"
#include <cstdlib>
#include <glm/ext/vector_float2.hpp>

PolygonBody::PolygonBody(Material mate, std::vector<glm::vec2> &vertices)
    : RigidBody(mate, PhysicsShapeType::POLYGON) {

    if (vertices.size() < 3) {
        throw std::invalid_argument("Polygon must have at least 3 vertices.");
    }

    OriginVertices = std::move(vertices);

    // check the direction of points
    GetArea();
    if (area_ < 0) {
        std::reverse(OriginVertices.begin(), OriginVertices.end());
    }
    GetCentroidPosition();

    mass_ = area_ * material_.density * TUMassFactor;
    massR_ = 1.0f / mass_;
    CalRotateInertia();

    TransformedVertices = std::vector<glm::vec2>(OriginVertices.size());
    TransformedVerticesB = std::vector<SDL_Vertex>(OriginVertices.size());
    TransformedVerticesF = std::vector<SDL_Vertex>(OriginVertices.size());

    SetFColor({1, 1, 0, 1});
    SetFColorBoundry({1, 1, 1, 1});
}

void PolygonBody::SetColor(SDL_Color color) {
    color_ = ToFColor(color);
    for (auto &Vertices : TransformedVerticesF) {
        Vertices.color = color_;
    }
}

void PolygonBody::SetFColor(SDL_FColor color) {
    color_ = color;
    for (auto &Vertices : TransformedVerticesF) {
        Vertices.color = color_;
    }
}

void PolygonBody::SetColorBoundry(SDL_Color color) {
    boundaryColor_ = ToFColor(color);
    for (auto &Vertices : TransformedVerticesB) {
        Vertices.color = boundaryColor_;
    }
}

void PolygonBody::SetFColorBoundry(SDL_FColor color) {
    boundaryColor_ = color;
    for (auto &Vertices : TransformedVerticesB) {
        Vertices.color = boundaryColor_;
    }
}

const std::vector<glm::vec2> &PolygonBody::GetVertices() {
    if (!needToTransfrom)
        return TransformedVertices;

    needToTransfrom = false;
    transformer.Reset(rotation_, position_);
    for (int i = 0; i < TransformedVertices.size(); i++) {
        TransformedVertices[i] = transformer.TransfromR(OriginVertices[i]);
    }
    return TransformedVertices;
}

const AABB &PolygonBody::GetAABB() {
    if (!needToUpdateAABB)
        return aabb_;

    needToUpdateAABB = false;
    // ensure the transform is up to date
    GetVertices();
    aabb_.maxP = TransformedVertices[0];
    aabb_.minP = aabb_.maxP;
    for (int i = 1; i < TransformedVertices.size(); i++) {
        aabb_.maxP.x = std::max(aabb_.maxP.x, TransformedVertices[i].x);
        aabb_.maxP.y = std::max(aabb_.maxP.y, TransformedVertices[i].y);
        aabb_.minP.x = std::min(aabb_.minP.x, TransformedVertices[i].x);
        aabb_.minP.y = std::min(aabb_.minP.y, TransformedVertices[i].y);
    }
    return aabb_;
}

void PolygonBody::Render() {
    for (int i = 0; i < TransformedVertices.size(); i++) {
        auto temp = ToFPoint(TransformedVertices[i]);
        TransformedVerticesF[i].position = temp;
        TransformedVerticesB[i].position = temp;
    }

    // draw it self
    DrawCommand cmd(ShapeType::POLYGON, false);
    cmd.GetComplex().data = TransformedVerticesF;
    GET_Buffer.AddCommand(std::move(cmd));

    cmd = DrawCommand(ShapeType::HOLLOW_POL, false);
    cmd.GetComplex().data = TransformedVerticesB;
    GET_Buffer.AddCommand(std::move(cmd));
}

void PolygonBody::CalRotateInertia() {
    // calculate the inertia of the polygon
    float I = 0.0f;
    for (int i = 0; i < OriginVertices.size(); i++) {
        auto p1 = OriginVertices[i];
        auto p2 = OriginVertices[(i + 1) % OriginVertices.size()];
        I += (p1.x * p2.y - p1.y * p2.x);
    }
    I = abs(I);
    rotateInertia_ = massR_ * I / 6.0f;
    rotateInertiaR_ = 1.0f / rotateInertia_;
}

// only use for direction detect
void PolygonBody::GetArea() {
    area_ = 0;
    for (int i = 0; i < OriginVertices.size(); i++) {
        glm::vec2 p1 = OriginVertices[i];
        glm::vec2 p2 = OriginVertices[(i + 1) % OriginVertices.size()];
        area_ += (p1.x * p2.y - p1.y * p2.x);
    }
    area_ = 0.5f * area_;
}

// cope from box2d
void PolygonBody::GetCentroidPosition() {

    glm::vec2 center = {0, 0};
    area_ = 0;

    glm::vec2 origin = OriginVertices[0];
    int count = OriginVertices.size() - 1;
    const float inv3 = 1.0f / 3.0f;

    for (int i = 1; i  < count; ++i) {
        glm::vec2 e1 = OriginVertices[i] - origin;
        glm::vec2 e2 = OriginVertices[i + 1] - origin;
        float a = 0.5f * Cross(e1, e2);

        center = center + a * inv3 * (e1 + e2);
        area_ += a;
    }

    center = center / area_;
    area_ = std::abs(area_);

    center = origin + center;

    for (auto &vertex : OriginVertices) {
        vertex -= center;
    }

    MoveTo(center); 
}