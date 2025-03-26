# pragma once

class EventSystem {
  public:
    static EventSystem& Instance();

    void HandleEvent();
};

# define GET_EventSystem EventSystem::Instance()