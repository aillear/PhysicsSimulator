#include "cord.h"
#include "configs.h"
#include "renderBufferMgr.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>
 
void CordConstraint::Solve(float dt) {
    if (bodyA == bodyB) return; 
    if (bodyA->GetIsStatic() && bodyB->GetIsStatic()) return;

    worldA = bodyA->LocalToWorld(anchorA);
    worldB = bodyB->LocalToWorld(anchorB);

    glm::vec2 ra = worldA - bodyA->GetPosition();
    glm::vec2 rb = worldB - bodyB->GetPosition();

    glm::vec2 raPerp = {-ra.y, ra.x};
    glm::vec2 rbPerp = {-rb.y, rb.x};

    glm::vec2 angularLinearVA = bodyA->GetAngularVelocity() * raPerp;
    glm::vec2 angularLinearVB = bodyB->GetAngularVelocity() * rbPerp;

    // va vb speed 
    glm::vec2 vA = bodyA->GetVelocity() + angularLinearVA;
    glm::vec2 vB = bodyB->GetVelocity() + angularLinearVB;
    glm::vec2 relativeV = vB - vA;

    glm::vec2 distance = worldB - worldA;
    float currentLength = glm::length(distance);
    
    isTight = currentLength > maxLength;
    if (!isTight) return; // no need to solve if not tight in rope restraint

    glm::vec2 norm = glm::normalize(distance);
    if (glm::all(glm::epsilonEqual(norm, ZeroVec2, 0.001f))) {
        norm = RightVec2;
    }

    float raPerpDotNorm = glm::dot(raPerp, norm);
    float rbPerpDotNorm = glm::dot(rbPerp, norm);

    float j = (currentLength - maxLength) * k * dt;
    glm::vec2 impulse = j * norm;

    // attention the note!
    bodyA->ApplyImpulse(impulse, ra);
    bodyB->ApplyImpulse(-impulse, rb);
}

void CordConstraint::Render() {
    DrawCommand cmd(ShapeType::LINE, false);
    cmd.GetBase().rect.p1 = worldA;
    cmd.GetBase().rect.p2 = worldB;
    cmd.GetBase().color = isTight ? Cord_Tight : Cord_Relaxed;
    cmd.halfLineWidth = 1.0f;
    GET_Buffer.AddCommand(std::move(cmd));
}