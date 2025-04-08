#include "object.h"
#include "SDL3/SDL_events.h"
#include <algorithm>
#include <memory>

ObjectID Object::objectCount = 0;

void Object::InitWrapper() {
    for (auto &callBack : initCallBacks)
        callBack();
    Init();
    for (auto &child : children)
        child->InitWrapper();
}

void Object::RenderWrapper() {
    if (!enabled)
        return;
    // first draw self, then draw children
    // this is important, because children may cover self
    Render();
    for (auto &child : children) {
        child->RenderWrapper();
    }
}

void Object::UpdateWrapper(float dt) {
    if (!enabled)
        return;
    for (auto &callBack : updateCallBacks) {
        callBack(dt);
    }
    Update(dt);
    for (auto &child : children) {
        child->UpdateWrapper(dt);
    }
}

void Object::PhysicsUpdateWrapper(float dt) {
    if (!enabled)
        return;
    for (auto &callBack : physicsUpdateCallBacks) {
        callBack(dt);
    }
    PhysicsUpdate(dt);
    for (auto &child : children) {
        child->PhysicsUpdateWrapper(dt);
    }
}

void Object::HandleEventWrapper(SDL_Event &event) {
    if (!enabled)
        return;
    for (auto &callBack : handleEventCallBacks)
        callBack(event);
    for (auto &child : children) {
        child->HandleEventWrapper(event);
    }
    HandleEvent(event);
}

void Object::DestroyWrapper() {
    for (auto &callBack : destroyCallBacks)
        callBack();
    for (auto &child : children) {
        child->DestroyWrapper();
    }
    Destroy();
}

void Object::CheckChildToRemove() {
    auto it = std::remove_if(children.begin(), children.end(), [](std::shared_ptr<Object>& obj) {
        if (!obj->removedMark) return false;
        obj->DestroyWrapper();
        obj->ReleaseAllChildren();
        return true;
    });
    children.erase(it, children.end());

    for (auto& obj : children) {
        obj->CheckChildToRemove();
    }
}

void Object::ReleaseAllChildren() {
    if (children.size() == 0)
        return;
    for (auto &chlid : children) {
        chlid->ReleaseAllChildren();
    }
    children.clear();
}

void Object::AddChild(std::shared_ptr<Object> child) {
    if (child->GetParent() != nullptr) {
        child->GetParent()->RemoveChild(child);
    }
    children.push_back(child);
    child->SetParent(this);
}

void Object::RemoveChild(std::shared_ptr<Object> child) {
    auto it = std::remove(children.begin(), children.end(), child);
    if (it != children.end()) {
        child->SetParent(nullptr);
        children.erase(it, children.end());
    }
}

void Object::RemoveChlidByID(ObjectID id) {
    auto it = std::remove_if(children.begin(), children.end(),
                             [id](const std::shared_ptr<Object> &child) {
                                 return child->objectID == id;
                             });
    if (it != children.end()) {
        (*it)->SetParent(nullptr);
        children.erase(it, children.end());
    }
}

void Object::RemoveChlidByName(const std::string &name) {
    auto it = std::remove_if(children.begin(), children.end(),
                             [&name](const std::shared_ptr<Object> &child) {
                                 return child->name == name;
                             });
    if (it != children.end()) {
        (*it)->SetParent(nullptr);
        children.erase(it, children.end());
    }
}

void Object::RemoveAllChildren() {
    for (auto &child : children) {
        child->SetParent(nullptr);
    }
    children.clear();
}

std::shared_ptr<Object> Object::GetChildByID(ObjectID id) {
    for (auto &child : children) {
        if (child->objectID == id) {
            return child;
        }
        auto foundChild = child->GetChildByID(id);
        if (foundChild) {
            return foundChild;
        }
    }
    return nullptr;
}

std::shared_ptr<Object> Object::GetChildByName(const std::string &name) {
    for (auto &child : children) {
        if (child->name == name) {
            return child;
        }
        auto foundChild = child->GetChildByName(name);
        if (foundChild) {
            return foundChild;
        }
    }
    return nullptr;
}

void Object::AddInitCallBack(BasicFunctionWrapper callBack) {
    initCallBacks.emplace_back(callBack);
}

void Object::AddRenderCallBack(BasicFunctionWrapper callBack) {
    renderCallBacks.emplace_back(callBack);
}

void Object::AddUpdateCallBack(UpdateFunctionWrapper callBack) {
    updateCallBacks.emplace_back(callBack);
}

void Object::AddPhysicsUpdateCallBack(UpdateFunctionWrapper callBack) {
    physicsUpdateCallBacks.emplace_back(callBack);
}

void Object::AddHandleEventCallBack(EventFunctionWrapper callBack) {
    handleEventCallBacks.emplace_back(callBack);
}

void Object::AddDestroyCallBack(BasicFunctionWrapper callBack) {
    destroyCallBacks.emplace_back(callBack);
}
