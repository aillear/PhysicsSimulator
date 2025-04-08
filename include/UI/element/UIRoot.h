#pragma once
#include "UIComponent.h"
#include "renderSystem.h"
#include <glm/ext/vector_float2.hpp>

/**
 * @brief root node for ui
 *
 */
class UIRoot : public UIComponent {
  public:
    glm::vec2 GetWidthHeight() const override {
        return GET_RenderSystem.GetWindowSize();
    }
  protected:
    void Init() override { this->leftTopPos = {0, 0}; }
    void Render() override { /* do nothing */;}
    void Update(float dt) override { /* do nothing */;}
};