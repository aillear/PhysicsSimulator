#include "boxBody.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "configs.h"
#include "conversion.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include "rigidbody.h"
#include "shape.h"
#include "transform.h"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float2.hpp>
#include <utility>
#include <vector>

BoxBody::BoxBody(Material mate, glm::vec2 widthHeight)
    : RigidBody(mate, PhysicsShapeType::BOX),
      widthHeight_(widthHeight) {
    area_ = widthHeight.x * widthHeight.y * TUAreaFactor;
    mass_ = area_ * material_.density * TUMassFactor;
    massR_ = 1.0f / mass_;

    float Right = widthHeight.x * 0.5f;
    float Buttom = widthHeight.y * 0.5f;
    OriginVertex[0] = {-Right, -Buttom};
    OriginVertex[1] = {Right, -Buttom};
    OriginVertex[2] = {Right, Buttom};
    OriginVertex[3] = {-Right, Buttom};
    TransformedVertexF = std::vector<SDL_Vertex>(4);
    TransformedVertexB = std::vector<SDL_Vertex>(4);
    
    // have to set color there, due to change color need to change vertex color
    SetFColor({1, 1, 0, 1});
    SetFColorBoundry({1, 1, 1, 1});
}

void BoxBody::SetColor(SDL_Color color) {
    color_ = ToFColor(color);
    for (auto& vertex : TransformedVertexF) {
        vertex.color = color_;
    }
}

void BoxBody::SetFColor(SDL_FColor color) {
    color_ = color;
    for (auto& vertex : TransformedVertexF) {
        vertex.color = color_;
    }
}

void BoxBody::SetColorBoundry(SDL_Color color) {
    boundaryColor_ = ToFColor(color);
    for (auto& vertex : TransformedVertexB) {
        vertex.color = boundaryColor_;
    }
}

void BoxBody::SetFColorBoundry(SDL_FColor color) {
    boundaryColor_ = color;
    for (auto& vertex : TransformedVertexB) {
        vertex.color = boundaryColor_;
    }
}

void BoxBody::GetVertexTransfrom() {
    if (!needToTransfrom) return;
    for (int i = 0; i < 4; i++) {
        auto afterTransformed = ToFPoint(transformer.TransfromR(OriginVertex[i]));
        TransformedVertexB[i].position = afterTransformed;
        TransformedVertexF[i].position = afterTransformed;
    } 
    needToTransfrom = false;
}

void BoxBody::GetAABBUpdated() {
    if (!needToUpdateAABB) return;
    
    aabb_.maxP = ToGlmVec2(TransformedVertexB.begin()->position);
    aabb_.minP = aabb_.maxP;
    for (int i = 1; i < TransformedVertexB.size(); i++) {
        aabb_.maxP.x = std::max(aabb_.maxP.x, TransformedVertexB[i].position.x);
        aabb_.maxP.y = std::max(aabb_.maxP.y, TransformedVertexB[i].position.y);
        aabb_.minP.x = std::min(aabb_.minP.x, TransformedVertexB[i].position.x);
        aabb_.minP.y = std::min(aabb_.minP.y, TransformedVertexB[i].position.y);
    }
    needToUpdateAABB = false;
}

void BoxBody::Render() {
    // draw it self
    DrawCommand cmd(ShapeType::POLYGON, false);
    cmd.GetComplex().data = TransformedVertexF;
    GET_Buffer.AddCommand(std::move(cmd));


    cmd = DrawCommand(ShapeType::HOLLOW_POL, false);
    cmd.GetComplex().data = TransformedVertexB;
    GET_Buffer.AddCommand(std::move(cmd));
}

void BoxBody::CalRotateIntertia() {
    // 0.833333f == 1/12
    // formula is : J = 1/12 * m * (w^2 + h^2)
    rotateIntertia_ = 0.833333f * mass_ * glm::dot(widthHeight_, widthHeight_);
    rotateIntertiaR_ = 1.0f / rotateIntertia_;
}


void BoxBody::PhysicsUpdate(float dt) {
    // SetRotation(rotation_ + 90*dt);
    RigidBody::PhysicsUpdate(dt);
}
