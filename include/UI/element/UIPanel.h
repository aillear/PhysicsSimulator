#pragma once

#include "SDL3/SDL_pixels.h"
#include "UIComponent.h"

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
TODO: inplement this!
*/

class UIPanel : public UIComponent {
  public:
    UIPanel(glm::vec2 leftTop = {0, 0}, glm::vec2 widthHeight = {0, 0},
            SDL_FColor color = {0, 0, 0, 0}, float barHeight = 20.0f,
            SDL_FColor barColor = {255, 255, 255, 255})
        : UIComponent(leftTop, widthHeight, color), barHeight(barHeight),
          barColor(barColor) {
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
    void SetBarHeight(int barHeight) { this->barHeight = barHeight; }
    SDL_FColor GetBarColor() const { return barColor; }
    void SetBarColor(SDL_FColor barColor) { this->barColor = barColor; }

  private:
    float barHeight;         // bar's height.
    SDL_FColor barColor;     // bar's color
    bool isDragging = false; // is the panel being dragged
    glm::vec2 lastPosition;  // last mouse position
};