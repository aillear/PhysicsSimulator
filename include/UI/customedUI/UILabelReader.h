#pragma once
#include "UILabel.h"
#include "eventSystem.h"
#include <string>

using ReaderCallBack = std::function<std::string()>;

// a label that link to some statistics.
class UILabelReader : public UILabel {
  public:
    UILabelReader(bool updateByTrigger = true) : updateByTrigger_(updateByTrigger) {;}
    void AddReader(ReaderCallBack &&callBack);

    void SetUpdateByTrigger(bool updateByTrigger) {
        updateByTrigger_ = updateByTrigger;
    }
    const bool GetUpdateByTrigger() const { return updateByTrigger_; }

  protected:
    void Init() override;
    void Update(float dt) override;
    void Destroy() override;

  private:
    bool updateByTrigger_ = false;
    ReaderCallBack callBack_ = nullptr;
    EventHandlerID eventHandlerID_ = 0;
};