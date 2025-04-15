#pragma once

#include "SDL3/SDL_pixels.h"
#include "UIComponent.h"
#include "conversion.h"
#include <glm/common.hpp>
#include <memory>

/**
 * @brief

a panel like this:
                    +-----------+
                    |    bar    |   <- bar height
relative point ->   +-----------+   <-+
                    |           |     |
                    |  content  |     |
                    |           |    height
                    |           |     |
                    |           |     |
                    |           |   <-+
                    +-----------+
                    ^           ^
                    +----width--+

relative point is left top corner of the panel.
bar is additional height of the panel, you can click it to drag the panel.
*/

class UIPanel : public UIComponent {
  public:
    UIPanel(glm::vec2 widthHeight = {0, 0}, float barHeight = 40.0f)
        : UIComponent(widthHeight), barHeight(barHeight) {
        ;
    }

    void Render() override;
    void Update(float dt) override;

    void OnMouseMove(SDL_Event &event) override;
    void OnMouseDown(SDL_Event &event) override;
    void OnMouseUp(SDL_Event &event) override;

    // hit test only test the bar area, for drag event.
    // detect these two points:
    //           +-----------+ <- p2
    //           |    bar    |
    //    p1 ->  +-----------+
    bool HitTest(glm::vec2 MousePos) override;

    float GetBarHeight() const { return barHeight; }

    virtual void SetBarHeight(float barHeight) {
        this->barHeight = std::max(barHeight, 40.0f);
    }

    SDL_FColor GetBarColor() const { return barColor; }
    void SetBarFColor(SDL_FColor barColor) { this->barColor = barColor; }
    void SetBarColor(SDL_Color barColor) {
        this->barColor = ToFColor(barColor);
    }

    // when using this method, please make sure the child is a real child of this panel.
    void SetBarAlignMent(std::shared_ptr<UIComponent> child, TextAlign xAlign,
                         TextAlign yAlign, glm::vec2 margin, glm::vec2 offset);
    // when using this method, please make sure the child is a real child of this panel.
    void SetBarAlignMent(UIComponent* child, TextAlign xAlign,
                         TextAlign yAlign, glm::vec2 margin, glm::vec2 offset);

  protected:
    float barHeight;         // bar's height.
    SDL_FColor barColor;     // bar's color
    bool isDragging = false; // is the panel being dragged
    glm::vec2 lastPosition;  // last mouse position
};