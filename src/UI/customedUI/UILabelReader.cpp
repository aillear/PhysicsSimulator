# include "UILabelReader.h"
#include "SDL3/SDL_events.h"
#include "configs.h"
#include "eventSystem.h"

void UILabelReader::Init() {
    if (callBack_ == nullptr) {
        callBack_ = []() { return std::string("no reader set"); };
    }

    eventHandlerID_ = GET_EventSystem.AddEventListener(USER_EVENT_SLOW_UPDATE, [this](SDL_Event &event) {
        if (this->updateByTrigger_) {
            this->ChangeText(callBack_());
        }
    });

}

void UILabelReader::AddReader(ReaderCallBack&& callBack) {
    callBack_ = std::move(callBack);
    if (callBack_ == nullptr) {
        callBack_ = []() { return std::string("no reader set"); };
    }
}

void UILabelReader::Update(float dt) {
    if (updateByTrigger_) return;

    if (callBack_ == nullptr) return;
    
    ChangeText(callBack_());
}

void UILabelReader::Destroy() {
    GET_EventSystem.RemoveEventListener(USER_EVENT_SLOW_UPDATE, eventHandlerID_);
    UILabel::Destroy();
}