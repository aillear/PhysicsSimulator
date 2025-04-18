#pragma once
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

struct Transform2D {
    Transform2D(glm::vec2 offset = {0, 0}, float angleDegree = 0)
        : offset_(offset), angle_(glm::radians(angleDegree)) {
        Update();
    }

    Transform2D(float angleRadian, glm::vec2 offset = {0, 0})
        : offset_(offset), angle_(angleRadian) {
        Update();
    }

    void Tranaform(glm::vec2 &target);
    glm::vec2 TransfromR(const glm::vec2 &target);
    void Reset(float angle, glm::vec2 offset);
    void SetAngle(float angle);
    void SetRAngle(float angle);
    void SetOffset(glm::vec2 offset) { offset_ = offset; }
    float GetAngle() { return glm::degrees(angle_); }
    float GetRAngle() const { return angle_; }
    glm::vec2 GetOffset() const { return offset_; }

  private:
    void Update();
    glm::mat2 rotation_matrix;
    glm::vec2 offset_;
    float angle_;
};