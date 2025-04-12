#pragma once

#include "objectWorld.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
class PhysicsObjectRoot : public ObjectWorld {
  protected:
    void Init() override { position_ = {0, 0}; }
    void Render() override;
    void PhysicsUpdate(float dt) override;
};