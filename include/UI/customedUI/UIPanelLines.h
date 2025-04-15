#include "UIComponent.h"
#include "UIPanelClose.h"
#include "object.h"
#include <memory>
#include <unordered_map>

class UIPanelLines : public UIPanelClose {
  public:
    UIPanelLines(glm::vec2 widthHeight = {0, 0}, float barHeight = 40.0f,
                 const std::string &title = "Lines Panel",
                 float LineIndent = 5.0f);

    void SetLineIndent(float lineIndent);
    const float GetLineIndent() const { return lineIndent_; }

    void AddNewContent(std::shared_ptr<UIComponent>&& child, TextAlign xAlign,
                    TextAlign yAlign, bool IsNewLine,
                    glm::vec2 margin = {0, 0});

    void RemoveChild(std::shared_ptr<Object> child) override;
    void RemoveChildByID(ObjectID id) override;
    void RemoveChildByName(const std::string &name) override;
    void RemoveAllChildren() override;

  protected:
    void ReAlignChildren();
    void Update(float dt) override;


    std::unordered_map<ObjectID, bool> isChildNewLine_;
    bool needToReAlignChildren_ = false;
    float lineIndent_;
};