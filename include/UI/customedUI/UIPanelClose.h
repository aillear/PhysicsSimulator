#pragma once

#include "UIButton.h"
#include "UILabel.h"
#include "UIPanel.h"
#include <glm/ext/vector_float2.hpp>

/**
 * @brief A panel that includes a close button and a title label.
 *        the button size will be set to barHeight - 10's square.
 *
 */
class UIPanelClose : public UIPanel {
  public:
    UIPanelClose(glm::vec2 widthHeight = {0, 0}, float barHeight = 40.0f,
                 const std::string &title = "Panel");

    UIButton *GetCloseButton() const { return closeButton_; }
    UILabel *GetTitleLabel() const { return titleLabel_; }

    void SetCloseBarAlignment(TextAlign xAlign, TextAlign yAlign,
                              glm::vec2 offset = {0, 0},
                              glm::vec2 margin = {0, 0});
    void SetTitleLabelAlignment(TextAlign xAlign, TextAlign yAlign,
                                glm::vec2 offset = {0, 0},
                                glm::vec2 margin = {0, 0});

    void SetBarHeight(float barHeight) override;


  protected:
    // don't need to release these.
    UIButton *closeButton_;
    UILabel *titleLabel_;
};
