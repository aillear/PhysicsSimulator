#pragma once

#include "SDL3/SDL_stdinc.h"
#include "conversion.h"
#include "object.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <glm/ext/vector_float2.hpp>

/**
 * @brief UI object's base class.
 *    provide basic hit test and get relative and absolute position methods.
 *    the topLeftPos means the position relate to it's ancestor's top left
 * corner it's in screen coordinate system. you can get absolute position in
 * screen by GetScreenPos(), it will recursively get all the parents' position
 * and add them up.
 *
 *   live cycle:
 *
 *  Init(when first added to the UIMgr) -> Update -> Render -> Destroy(when
 * removed from the UIMgr)
 *
 *  it also provide a basic layout system. you can see it in the guide.md
 */

enum class TextAlign : Uint8 { DEFAULT = 0, START, CENTER, END };
class UIComponent : public Object {
  public:
    // TODO:only handle mouse event? may change in the future.
    void HandleEvent(SDL_Event &event) override;

    bool GetZIndex() const { return zIndex; }
    void SetZIndex(bool zIndex) { this->zIndex = zIndex; }

    SDL_FColor GetColor() const { return color; }
    void SetFColor(SDL_FColor color) { this->color = color; }
    void SetColor(SDL_Color color) { this->color = ToFColor(color); }
    bool GetRelateMode() const { return relateToParent; }
    void SetRelateMode(bool relateToParent) {
        this->relateToParent = relateToParent;
    }

    // normally, you should not use these methods, unless you know what you are
    // doing. using set pos will not change the alignment field.
    glm::vec2 GetRelativePos() const { return leftTopPos; }
    glm::vec2 GetScreenPos() const;
    void SetRelativePos(glm::vec2 pos) { leftTopPos = pos; }
    virtual glm::vec2 GetWidthHeight() const { return widthHeight; }
    void SetWidthHeight(glm::vec2 widthHeight) {
        this->widthHeight = widthHeight;
    }

    glm::vec2 GetParentRelativePos() const;
    glm::vec2 GetParentScreenPos() const;
    virtual glm::vec2 GetParentWidthHeight() const;

    glm::vec2 GetMargin() const { return margin; }
    float GetMarginX() const { return margin.x; }
    float GetMarginY() const { return margin.y; }
    glm::vec2 GetOffset() const { return offset; }
    float GetOffsetX() const { return offset.x; }
    float GetOffsetY() const { return offset.y; }
    TextAlign GetXAlign() const { return xAlign_; }
    TextAlign GetYAlign() const { return yAlign_; }
    // those set methods will change the position of the component.
    void SetAlignMent(TextAlign xAlign, TextAlign yAlign,
                      glm::vec2 margin = {0, 0}, glm::vec2 offset = {0, 0});
    void SetMargin(glm::vec2 margin);
    void SetMarginX(float marginX);
    void SetMarginY(float marginY);
    void SetOffset(glm::vec2 offset);
    void SetOffsetX(float offsetX);
    void SetOffsetY(float offsetY);
    void SetAlignModeX(TextAlign xAlign);
    void SetAlignModeY(TextAlign yAlign);

    virtual bool HitTest(glm::vec2 MousePos);

    // so that it's children don't need to implement these functions

    UIComponent(glm::vec2 widthHeight = {0, 0});

  protected:
    void Render() override = 0;
    void Update(float dt) override = 0;
    void Init() override { ; }
    void Destroy() override { ; }
    void PhysicsUpdate(float dt) override { ; }
    void OnMouseMove(SDL_Event &event) override { ; }
    void OnMouseDown(SDL_Event &event) override { ; }
    void OnMouseUp(SDL_Event &event) override { ; }
    void OnKeyUp(SDL_Event &event) override { ; }
    void OnKeyDown(SDL_Event &event) override { ; }
    void ImplementAlignment();

    // useless for now
    bool zIndex;
    bool relateToParent = true; // if true, the position is relative to parent,
                                // false relative to screen(0, 0)
    TextAlign xAlign_;
    TextAlign yAlign_;
    // this rect is screen pos!!!
    glm::vec2 leftTopPos;
    glm::vec2 widthHeight;
    // margin to place uicoponent, use it will change position.
    glm::vec2 margin;
    glm::vec2 offset;
    SDL_FColor color;

  private:
    bool needToImplementAlignment =
        false; // if true, need to implement alignment
};