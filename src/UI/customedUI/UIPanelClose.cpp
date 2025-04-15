#include "UIPanelClose.h"
#include "UIButton.h"
#include "UIMgr.h"
#include "UIPanel.h"
#include "logger.h"
#include <glm/common.hpp>
#include <glm/ext/vector_bool4.hpp>
#include <string>

UIPanelClose::UIPanelClose(glm::vec2 widthHeight, float barHeight,
                           const std::string &title)
    : UIPanel(widthHeight, barHeight) {
    // do not change their relationship, or may cause wild pointer reference!.
    closeButton_ = new UIButton({barHeight - 10, barHeight - 10});
    titleLabel_ = new UILabel(title);

    bool &thisEnable = this->enabled;
    closeButton_->SetCallBack(
        [&thisEnable](SDL_Event &event) { thisEnable = false; });

    SetBarAlignMent(closeButton_, TextAlign::END, TextAlign::CENTER, {5, 0}, {0, 0});
    F_LOG_DEBUG("UIPanelClose: close button's size: {}",
                closeButton_->GetWidthHeight());
    SetBarAlignMent(titleLabel_, TextAlign::START, TextAlign::CENTER, {10, 0}, {0, 0});

    closeButton_->SetEnabled(true);
    titleLabel_->SetEnabled(true);

    GET_UIMgr.AddUIComponent(closeButton_, this);
    GET_UIMgr.AddUIComponent(titleLabel_, this);
}

void UIPanelClose::SetCloseBarAlignment(TextAlign xAlign, TextAlign yAlign,
                                        glm::vec2 offset, glm::vec2 margin) {
    SetBarAlignMent(closeButton_, xAlign, yAlign, offset, margin);
}

void UIPanelClose::SetTitleLabelAlignment(TextAlign xAlign, TextAlign yAlign,
                                          glm::vec2 offset, glm::vec2 margin) {
    SetBarAlignMent(titleLabel_, xAlign, yAlign, offset, margin);
}

void UIPanelClose::SetBarHeight(float barHeight) {
    barHeight = std::max(barHeight, 40.0f);
    closeButton_->SetWidthHeight({barHeight - 5, barHeight - 5});
}