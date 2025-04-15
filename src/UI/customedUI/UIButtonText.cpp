#include "UIButtonText.h"
#include "UIButton.h"
#include "UIComponent.h"
#include "UIMgr.h"

UIButtonText::UIButtonText(glm::vec2 widthHeight, const std::string &text)
    : UIButton(widthHeight) {
    label_ = new UILabel(text);
    label_->SetAlignMent(TextAlign::CENTER, TextAlign::CENTER);

    label_->SetEnabled(true);
    // add to UIMgr
    GET_UIMgr.AddUIComponent(label_, this);
}

void UIButtonText::SetLabelAlignment(TextAlign xAlign, TextAlign yAlign,
                                   glm::vec2 offset, glm::vec2 margin) {
    label_->SetAlignMent(xAlign, yAlign, offset, margin);
}

