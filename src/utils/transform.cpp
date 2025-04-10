#include "transform.h"
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/trigonometric.hpp>

void Transform2D::Update() {
    float rAngle = glm::radians(angle_);
    float cosA = glm::cos(rAngle);
    float sinA = glm::sin(rAngle);
    rotation_matrix = glm::mat2(cosA, -sinA, sinA, cosA);
}

void Transform2D::SetAngle(float angle) {
    angle_ = angle;
    Update();
}

void Transform2D::Tranaform(glm::vec2& target) {
    target = rotation_matrix * target + offset_;
}

glm::vec2 Transform2D::TransfromR(const glm::vec2& target) {
     return rotation_matrix * target + offset_;
}