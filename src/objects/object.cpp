# include "object.h"

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

void Object::RemoveChlidByID(Uint32 id) {
    auto it = std::remove_if(children.begin(), children.end(),
                             [id](const std::shared_ptr<Object> &child) { return child->objectID == id; });
    if (it != children.end()) {
        (*it)->SetParent(nullptr);
        children.erase(it, children.end());
    }
}

void Object::RemoveChlidByName(const std::string &name) {
    auto it = std::remove_if(children.begin(), children.end(),
                             [&name](const std::shared_ptr<Object> &child) { return child->name == name; });
    if (it != children.end()) {
        (*it)->SetParent(nullptr);
        children.erase(it, children.end());
    }
}

void Object::ClearChildren() {
    for (auto &child : children) {
        child->SetParent(nullptr);
    }
    children.clear();
}

std::shared_ptr<Object> Object::GetChildByID(Uint32 id) {
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