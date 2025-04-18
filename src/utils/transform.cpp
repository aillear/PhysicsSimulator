#include "transform.h"
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/trigonometric.hpp>

void Transform2D::Update() {
    float cosA = glm::cos(angle_);
    float sinA = glm::sin(angle_);
    rotation_matrix = glm::mat2(cosA, -sinA, sinA, cosA);
}

void Transform2D::Reset(float angleRadian, glm::vec2 offset) {
    offset_ = offset;
    angle_ = angleRadian;
    Update();
}

void Transform2D::SetAngle(float angle) {
    angle_ = glm::radians(angle);
    Update();
}

void Transform2D::SetRAngle(float angle) {
    angle_ = angle;
    Update();
}


void Transform2D::Tranaform(glm::vec2& target) {
    target = target * rotation_matrix + offset_;
}

glm::vec2 Transform2D::TransfromR(const glm::vec2& target) {
     return target * rotation_matrix + offset_;
}