#pragma once
#include "UILabel.h"
#include <string>

using ReaderCallBack = std::function<std::string()>;

// a label that link to some statistics.
class UILabelReader : public UILabel {
  public:
    void AddReader(ReaderCallBack callBack) { callBack_ = callBack; }

  protected:
    void Init() override {
        if (callBack_ == nullptr) {
          callBack_ = []() { return std::string("no reader set");};
        }
    }
    void Update(float dt) override { this->ChangeText(callBack_()); }

  private:
    ReaderCallBack callBack_ = nullptr;
};