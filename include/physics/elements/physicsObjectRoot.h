# pragma once   

#include "objectWorld.h"
class PhysicsObjectRoot : public ObjectWorld{
    protected:
        void Init() override {position_ = {0, 0};}
        void Render() override {;}
        void PhysicsUpdate(float dt) override {;}
};