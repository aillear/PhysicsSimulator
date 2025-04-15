#include "UIPanelLines.h"
#include "UIComponent.h"
#include "UIMgr.h"
#include "logger.h"
#include "object.h"
#include <_mingw_stat64.h>

UIPanelLines::UIPanelLines(glm::vec2 widthHeight, float barHeight,
                           const std::string &title, float LineIndent)
    : UIPanelClose(widthHeight, barHeight, title), lineIndent_(LineIndent) {

    if (lineIndent_ < 0) {
        lineIndent_ = 0;
    }

}

void UIPanelLines::AddNewContent(std::shared_ptr<UIComponent> &&child,
                                 TextAlign xAlign, TextAlign yAlign,
                                 bool IsNewLine, glm::vec2 margin) {
    if (child == nullptr) {
        LOG_ERROR("UIPanelLines::AddNewContent: child is nullptr.");
        return;
    }

    needToReAlignChildren_ = true;
    isChildNewLine_[child->GetID()] = IsNewLine;
    child->SetAlignMent(xAlign, yAlign, margin);
    GET_UIMgr.AddUIComponent(child, this);
}

void UIPanelLines::ReAlignChildren() {
    // 4 combination and 2 directions
    float offsetsCounter[4][2] = {0};
    for (auto &child : children) {

        if (child == nullptr || !child->GetEnabled())
            continue;
        if (child->GetID() == closeButton_->GetID() ||
            child->GetID() == titleLabel_->GetID()) {
            continue;
        }

        auto t = static_cast<UIComponent *>(child.get());

        if (isChildNewLine_[child->GetID()]) {
            if (t->GetXAlign() == TextAlign::START &&
                t->GetYAlign() == TextAlign::START) {

                t->SetOffsetY(offsetsCounter[0][1]);
                offsetsCounter[0][1] += t->GetWidthHeight().y + lineIndent_;

            } else if (t->GetXAlign() == TextAlign::END &&
                       t->GetYAlign() == TextAlign::START) {

                t->SetOffsetY(offsetsCounter[1][1]);
                offsetsCounter[1][1] += t->GetWidthHeight().y + lineIndent_;
            } else if (t->GetXAlign() == TextAlign::START &&
                       t->GetYAlign() == TextAlign::END) {

                t->SetOffsetY(offsetsCounter[2][1]);
                offsetsCounter[2][1] += t->GetWidthHeight().y + lineIndent_;

            } else if (t->GetXAlign() == TextAlign::END &&
                       t->GetYAlign() == TextAlign::END) {

                t->SetOffsetY(offsetsCounter[3][1]);
                offsetsCounter[3][1] += t->GetWidthHeight().y + lineIndent_;
            }
        } else {
            if (t->GetXAlign() == TextAlign::START &&
                t->GetYAlign() == TextAlign::START) {

                t->SetOffsetX(offsetsCounter[0][0]);
                offsetsCounter[0][0] += t->GetWidthHeight().x + lineIndent_;

            } else if (t->GetXAlign() == TextAlign::END &&
                       t->GetYAlign() == TextAlign::START) {

                t->SetOffsetX(offsetsCounter[1][0]);
                offsetsCounter[1][0] += t->GetWidthHeight().x + lineIndent_;
            } else if (t->GetXAlign() == TextAlign::START &&
                       t->GetYAlign() == TextAlign::END) {

                t->SetOffsetX(offsetsCounter[2][0]);
                offsetsCounter[2][0] += t->GetWidthHeight().x + lineIndent_;
            } else if (t->GetXAlign() == TextAlign::END &&
                       t->GetYAlign() == TextAlign::END) {

                t->SetOffsetX(offsetsCounter[3][0]);
                offsetsCounter[3][0] += t->GetWidthHeight().x + lineIndent_;
            }
        }
    }
}

void UIPanelLines::RemoveChild(std::shared_ptr<Object> child) {
    Object::RemoveChild(child);
    needToReAlignChildren_ = true;
    isChildNewLine_.erase(child->GetID());
}

void UIPanelLines::RemoveChildByID(ObjectID id) {
    Object::RemoveChildByID(id);
    needToReAlignChildren_ = true;
}

void UIPanelLines::RemoveChildByName(const std::string &name) {
    auto t = GetChildByName(name);
    if (t == nullptr)
        return;
    // DO NOT DELETE THIS
    isChildNewLine_.erase(t->GetID());
    RemoveChild(t);
}

void UIPanelLines::RemoveAllChildren() {
    Object::RemoveAllChildren();
    needToReAlignChildren_ = true;
}

void UIPanelLines::Update(float dt) {
    if (needToReAlignChildren_) {
        ReAlignChildren();
        needToReAlignChildren_ = false;
    }
}