# pragma once
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform2D {
    Transform2D(glm::vec2 offset, float angle) : offset_(offset), angle_(angle) {
        Update();
    }

    void Tranaform(glm::vec2& target);
    glm::vec2 TransfromR(const glm::vec2& target);
    void SetAngle(float angle);
    void SetOffset(glm::vec2 offset) { offset_ = offset;}
    float GetAngle() const {return angle_;}
    glm::vec2 GetOffset() const {return offset_;} 
  private:
    void Update();
    glm::mat2 rotation_matrix;
    glm::vec2 offset_;
    float angle_;
};