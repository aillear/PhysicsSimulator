#include "physicsSystem.h"
#include "SDL3/SDL_scancode.h"
#include "collisionMgr.h"
#include "configs.h"
#include "conversion.h"
#include "inputSystem.h"
#include "logger.h"
#include "object.h"
#include "objectWorld.h"
#include "physicsObjectRoot.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include "rigidbody.h"
#include "shape.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vector_relational.hpp>
#include <stdexcept>

using Shape = PhysicsShapeType;

bool SortByAABB(const std::shared_ptr<Object> &a,
                const std::shared_ptr<Object> &b) {
    auto pa = static_cast<RigidBody *>(a.get());
    auto pb = static_cast<RigidBody *>(b.get());
    return pa->GetAABB().minP.x < pb->GetAABB().minP.x;
}

PhysicsSystem &PhysicsSystem::Instance() {
    static PhysicsSystem instance;
    return instance;
}

bool PhysicsSystem::Init(int iteration) {

    rootNode = std::make_shared<PhysicsObjectRoot>();
    rootNode->SetName("rootPhysicsNode");

    // add event handler, subscribe 6 event
    eventHandler_1 = GET_EventSystem.AddEventListener(
        SDL_EVENT_QUIT, [this](SDL_Event &) { this->running = false; });

    eventHandler_2 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    eventHandler_3 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    eventHandler_4 = GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_UP,
        [this](SDL_Event &event) { HandleSDLEvents(event); });

    if (iteration < MinIteration || iteration > MaxIteration) {
        F_LOG_ERROR("PhysicsSystem: iteration is out of range, it must be "
                    "between {} and {}",
                    MinIteration, MaxIteration);
        throw std::out_of_range("iteration is out of range.");
    }
    this->iteration_ = iteration;
    return true;
}

void PhysicsSystem::Destroy() {
    // release all event
    GET_EventSystem.RemoveEventListener(SDL_EVENT_QUIT, eventHandler_1);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_MOTION, eventHandler_2);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_DOWN,
                                        eventHandler_3);
    GET_EventSystem.RemoveEventListener(SDL_EVENT_MOUSE_BUTTON_UP,
                                        eventHandler_4);
}

void PhysicsSystem::UpdateWrapper() {
    for (auto &callBack : initFunctionWrapper)
        callBack();
    while (running) {
        Update();
    }
    LOG_INFO("physics cycle is end.");
}

void PhysicsSystem::Update() {
    fpsc.StartFrame();

    ObjManage();

    float dt = fpsc.GetLastFrameSecond() / iteration_;
    if (PKeyDown(SDL_SCANCODE_SPACE)) {
        int a;
    }
    // execute iteration here.
    for (int i = 0; i < iteration_; i++) {
        collisionPairs.clear();
        rootNode->PhysicsUpdateWrapper(dt);
        ConllisionBroadPhase();
        ConllisionNarrowPhase();
    }
    OutOffBoundCheck();

    for (auto &callBack : AfterUpdateFunctionWrapper)
        callBack();

    // render here
    rootNode->RenderWrapper();

    for (auto &point : collisionPoints) {
        DrawCommand cmd(ShapeType::HOLLOW_RECT, false);
        cmd.GetBase().rect.p1 = point - glm::vec2(0.05f, 0.05f);
        cmd.GetBase().rect.p2 = point + glm::vec2(0.05f, 0.05f);
        cmd.GetBase().color = {1, 0, 1, 1};
        cmd.halfLineWidth = 0.5f;
        GET_Buffer.AddCommand(std::move(cmd));
    }
    collisionPoints.clear();
    GET_Buffer.Submit();

    fpsc.EndFrame();
}

void PhysicsSystem::ObjManage() {
    if (hasRemoveCalled) {
        rootNode->CheckChildToRemove();
        hasRemoveCalled = false;
    }

    for (auto &obj : physicsObjectsToAdd) {
        auto parent = obj->GetTempFather();
        if (parent == nullptr) {
            rootNode->AddChild(obj);
        } else
            parent->AddChild(obj);
        obj->InitWrapper();
    }
    physicsObjectsToAdd.clear();
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj,
                              std::shared_ptr<ObjectWorld> target) {
    obj->tempFather = target.get();
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj,
                              ObjectID targetID) {
    Object *target = FindObjectById(targetID).get();
    obj->tempFather = target;
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::AddObject(std::shared_ptr<ObjectWorld> obj,
                              std::string targetName) {
    Object *target = FindObjectByName(targetName).get();
    obj->tempFather = target;
    physicsObjectsToAdd.push_back(obj);
}

void PhysicsSystem::RemoveObject(std::shared_ptr<Object> obj) {
    obj->SetToRemove();
    hasRemoveCalled = true;
}

void PhysicsSystem::RemoveObject(ObjectID name) {
    auto removeObj = FindObjectById(name);
    removeObj->SetToRemove();
    hasRemoveCalled = false;
}

void PhysicsSystem::RemoveObject(std::string name) {
    auto removeObj = FindObjectByName(name);
    removeObj->SetToRemove();
    hasRemoveCalled = false;
}

void PhysicsSystem::HandleSDLEvents(SDL_Event &event) {
    rootNode->HandleEventWrapper(event);
}

std::shared_ptr<ObjectWorld> PhysicsSystem::FindObjectById(ObjectID id) {
    for (auto &comp : physicsObjectsToAdd) {
        if (comp->GetID() == id)
            return comp;
    }
    return std::static_pointer_cast<ObjectWorld>(rootNode->GetChildByID(id));
}

std::shared_ptr<ObjectWorld> PhysicsSystem::FindObjectByName(std::string name) {
    for (auto &comp : physicsObjectsToAdd) {
        if (comp->GetName() == name)
            return comp;
    }
    return std::static_pointer_cast<ObjectWorld>(
        rootNode->GetChildByName(name));
}

void PhysicsSystem::ConllisionBroadPhase() {
    auto children = rootNode->GetChildren();
    int childCount = children.size();

    for (int i = 0; i < childCount - 1; i++) {
        RigidBody *objA = static_cast<RigidBody *>(children[i].get());
        AABB aabbA = objA->GetAABB();
        for (int j = i + 1; j < childCount; j++) {
            RigidBody *objB = static_cast<RigidBody *>(children[j].get());

            if (objA->GetIsStatic() && objB->GetIsStatic())
                continue;

            AABB aabbB = objB->GetAABB();

            if (!GET_CollisionMgr.IntersectAABBs(aabbA, aabbB))
                continue;

            collisionPairs.emplace_back(i, j);
        }
    }
}

void PhysicsSystem::ConllisionNarrowPhase() {
    auto children = rootNode->GetChildren();

    glm::vec2 norm;
    float depth;
    for (auto [i, j] : collisionPairs) {
        RigidBody *objA = static_cast<RigidBody *>(children[i].get());
        RigidBody *objB = static_cast<RigidBody *>(children[j].get());

        if (!GET_CollisionMgr.CollisionCheck(objA, objB, norm, depth))
            continue;

        SeperateBodies(objA, objB, norm * depth);
        Collision c{objA, objB, norm, depth, {0, 0}, {0, 0}, 0};
        GET_CollisionMgr.FindContactPoints(objA, objB, c.point1, c.point2,
                                           c.count);
        FFCollisionResolver(c);

        // debug
        collisionPoints.push_back(c.point1);
        if (c.count == 2) {
            collisionPoints.push_back(c.point2);
        }
    }
}

void PhysicsSystem::SeperateBodies(RigidBody *objA, RigidBody *objB,
                                   glm::vec2 mtv) {
    if (objA->GetIsStatic()) {
        objB->Move(mtv);
    } else if (objB->GetIsStatic()) {
        objA->Move(-mtv);
    } else {
        auto ds = mtv * 0.5f;
        objA->Move(-ds);
        objB->Move(ds);
    }
}

void PhysicsSystem::CollisionResolver(const Collision &collision) {
    auto a = collision.objA;
    auto b = collision.objB;
    auto norm = collision.norm;
    auto depth = collision.depth;

    if (a == nullptr || b == nullptr) {
        LOG_ERROR("nullptr in collission resolver.");
        throw std::runtime_error("null ptr in collision resolver");
    }

    glm::vec2 relativeV = b->GetVelocity() - a->GetVelocity();
    float reVdotNorm = glm::dot(relativeV, norm);
    if (reVdotNorm > 0.0f)
        return;

    float resilience =
        std::min(a->GetMaterial().resilience, b->GetMaterial().resilience);

    float j = -(1.0f + resilience) * reVdotNorm;
    j /= a->GetMassR() + b->GetMassR();

    glm::vec2 impulse = j * norm;

    a->AddVelocity(-a->GetMassR() * impulse);
    b->AddVelocity(b->GetMassR() * impulse);
}

void PhysicsSystem::FCollisionResolver(const Collision &collision) {
    auto objA = collision.objA;
    auto objB = collision.objB;
    auto norm = collision.norm;
    auto contactCount = collision.count;

    if (objA == nullptr || objB == nullptr) {
        LOG_ERROR("nullptr in collission resolver.");
        throw std::runtime_error("null ptr in collision resolver");
    }

    auto resilience = std::min(objA->GetMaterial().resilience,
                               objB->GetMaterial().resilience);

    std::array<glm::vec2, 2> contactPoints{collision.point1, collision.point2};
    std::array<glm::vec2, 2> impulses{};
    std::array<glm::vec2, 2> raList{};
    std::array<glm::vec2, 2> rbList{};

    for (int i = 0; i < contactCount; i++) {
        glm::vec2 ra = contactPoints[i] - objA->GetPosition();
        glm::vec2 rb = contactPoints[i] - objB->GetPosition();
        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVA = objA->GetAngularVelocity() * raPerp;
        glm::vec2 angularLinearVB = objB->GetAngularVelocity() * rbPerp;

        glm::vec2 relativeV = (objB->GetVelocity() + angularLinearVB) -
                              (objA->GetVelocity() + angularLinearVA);

        float reVdotNorm = glm::dot(relativeV, norm);
        if (reVdotNorm > 0.0f) {
            continue;
        }

        float raPerpDotNorm = glm::dot(raPerp, norm);
        float rbPerpDotNorm = glm::dot(rbPerp, norm);

        float denom =
            objA->GetMassR() + objB->GetMassR() +
            (raPerpDotNorm * raPerpDotNorm) * objA->GetRotateIntertiaR() +
            (rbPerpDotNorm * rbPerpDotNorm) * objB->GetRotateIntertiaR();

        float j = -(1.0f + resilience) * reVdotNorm;
        j /= denom;
        j /= contactCount;

        impulses[i] = j * norm;
    }

    for (int i = 0; i < contactCount; i++) {
        glm::vec2 impulse = impulses[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        objA->AddVelocity(-impulse * objA->GetMassR());
        objA->AddAngularVelocity(-Cross(ra, impulse) *
                                 objA->GetRotateIntertiaR());

        objB->AddVelocity(impulse * objB->GetMassR());
        objB->AddAngularVelocity(Cross(rb, impulse) *
                                 objB->GetRotateIntertiaR());
    }
}

void PhysicsSystem::FFCollisionResolver(const Collision &collision) {
    auto objA = collision.objA;
    auto objB = collision.objB;
    auto norm = collision.norm;
    auto contactCount = collision.count;

    if (objA == nullptr || objB == nullptr) {
        LOG_ERROR("nullptr in collission resolver.");
        throw std::runtime_error("null ptr in collision resolver");
    }

    auto resilience = std::min(objA->GetMaterial().resilience,
                               objB->GetMaterial().resilience);

    float sf = (objA->GetMaterial().StaticfrictionFactor +
                objB->GetMaterial().StaticfrictionFactor) *
               0.5f;
    float df = (objA->GetMaterial().DynamicfrictionFactor +
                objB->GetMaterial().DynamicfrictionFactor) *
               0.5f;

    std::array<glm::vec2, 2> contactPoints{collision.point1, collision.point2};
    std::array<glm::vec2, 2> frictionImpulses{};
    std::array<glm::vec2, 2> impulses{};
    std::array<glm::vec2, 2> raList{};
    std::array<glm::vec2, 2> rbList{};
    std::array<float, 2> jList{};

    for (int i = 0; i < contactCount; i++) {
        glm::vec2 ra = contactPoints[i] - objA->GetPosition();
        glm::vec2 rb = contactPoints[i] - objB->GetPosition();
        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVA = objA->GetAngularVelocity() * raPerp;
        glm::vec2 angularLinearVB = objB->GetAngularVelocity() * rbPerp;

        glm::vec2 relativeV = (objB->GetVelocity() + angularLinearVB) -
                              (objA->GetVelocity() + angularLinearVA);

        float reVdotNorm = glm::dot(relativeV, norm);
        if (reVdotNorm > 0.0f) {
            continue;
        }

        float raPerpDotNorm = glm::dot(raPerp, norm);
        float rbPerpDotNorm = glm::dot(rbPerp, norm);

        float denom =
            objA->GetMassR() + objB->GetMassR() +
            (raPerpDotNorm * raPerpDotNorm) * objA->GetRotateIntertiaR() +
            (rbPerpDotNorm * rbPerpDotNorm) * objB->GetRotateIntertiaR();

        float j = -(1.0f + resilience) * reVdotNorm;
        j /= denom;
        j /= contactCount;
        jList[i] = j;
        impulses[i] = j * norm;
    }

    for (int i = 0; i < contactCount; i++) {
        glm::vec2 impulse = impulses[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        objA->AddVelocity(-impulse * objA->GetMassR());
        objA->AddAngularVelocity(-Cross(ra, impulse) *
                                 objA->GetRotateIntertiaR());

        objB->AddVelocity(impulse * objB->GetMassR());
        objB->AddAngularVelocity(Cross(rb, impulse) *
                                 objB->GetRotateIntertiaR());
    }
    
    for (int i = 0; i < contactCount; i++) {
        glm::vec2 ra = contactPoints[i] - objA->GetPosition();
        glm::vec2 rb = contactPoints[i] - objB->GetPosition();
        raList[i] = ra;
        rbList[i] = rb;

        glm::vec2 raPerp = {-ra.y, ra.x};
        glm::vec2 rbPerp = {-rb.y, rb.x};

        glm::vec2 angularLinearVA = objA->GetAngularVelocity() * raPerp;
        glm::vec2 angularLinearVB = objB->GetAngularVelocity() * rbPerp;

        glm::vec2 relativeV = (objB->GetVelocity() + angularLinearVB) -
                              (objA->GetVelocity() + angularLinearVA);

        glm::vec2 tagent = relativeV - glm::dot(relativeV, norm) * norm;
        if (glm::all(glm::epsilonEqual(tagent, {0, 0}, 0.001f))) {
            continue;
        }
        tagent = glm::normalize(tagent);

        float raPerpDotTangent = glm::dot(raPerp, tagent);
        float rbPerpDotTangent = glm::dot(rbPerp, tagent);

        float denom = objA->GetMassR() + objB->GetMassR() +
                      (raPerpDotTangent * raPerpDotTangent) *
                          objA->GetRotateIntertiaR() +
                      (rbPerpDotTangent * rbPerpDotTangent) *
                          objB->GetRotateIntertiaR();

        float jt = -glm::dot(relativeV, tagent);
        jt /= denom;
        jt /= contactCount;

        glm::vec2 frictionImpulse;
        float j = jList[i];
        if (abs(jt) < j * sf) frictionImpulse = jt * tagent;
        else
            frictionImpulse = -j * df * tagent;
        frictionImpulses[i] = frictionImpulse;
    }

    for (int i = 0; i < contactCount; i++) {
        glm::vec2 impulse = frictionImpulses[i];
        glm::vec2 ra = raList[i];
        glm::vec2 rb = rbList[i];

        objA->AddVelocity(-impulse * objA->GetMassR());
        objA->AddAngularVelocity(-Cross(ra, impulse) *
                                 objA->GetRotateIntertiaR());

        objB->AddVelocity(impulse * objB->GetMassR());
        objB->AddAngularVelocity(Cross(rb, impulse) *
                                 objB->GetRotateIntertiaR());
    }
}

// just for temp test
void PhysicsSystem::OutOffBoundCheck() {
    for (auto &child : rootNode->GetChildren()) {
        RigidBody *obj = static_cast<RigidBody *>(child.get());
        auto aabb = obj->GetAABB();

        if (aabb.minP.x > 4000 || aabb.maxP.x < -4000 || aabb.minP.y > 2320 ||
            aabb.maxP.y < -2320) {
            F_LOG_INFO("object id: {} out of bound, remove it.", obj->GetID());
            obj->SetToRemove();
            hasRemoveCalled = true;
        }
    }
}
