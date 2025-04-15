#pragma once
#include "UIButton.h"
#include "UIComponent.h"
#include "UILabel.h"
#include <string>

class UIButtonText : public UIButton {

  public:
    UIButtonText(glm::vec2 widthHeight = {0, 0},
                 const std::string &text = "button");

    const UILabel *GetLabel() const { return label_; }
    void SetLabelAlignment(TextAlign xAlign, TextAlign yAlign,
                           glm::vec2 offset = {0, 0},
                           glm::vec2 margin = {0, 0});

    void SetLabelText(const std::string &text) { label_->ChangeText(text); }
    void SetLabelFColor(SDL_FColor color) { label_->SetFColor(color); }
    void SetLabelColor(SDL_Color color) { label_->SetColor(color); }
    
  private:
    UILabel *label_;
};