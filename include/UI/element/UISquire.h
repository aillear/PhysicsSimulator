# pragma once   
#include "UIComponent.h"
#include <glm/ext/vector_float2.hpp>

/**
 * @brief rect like ui component base useless
 * 
 */ 
class UISquire : public UIComponent {
  public:
    void SetLeftTop(glm::vec2 leftTop) { this->leftTop = leftTop; }
    void SetRightBottom(glm::vec2 rightBottom) { this->rightBottom = rightBottom; }
    void SetRect(glm::vec2 leftTop, glm::vec2 rightBottom) {
        this->leftTop = leftTop;
        this->rightBottom = rightBottom;
    }

  protected:
    glm::vec2 leftTop;
    glm::vec2 rightBottom;
};