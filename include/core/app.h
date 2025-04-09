#pragma once
#include "FPSCounter.h"
#include <SDL3/SDL.h>
#include <SDL3_framerate.h>
#include <thread>
class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void Run();
    void Destroy();
    bool running;
    FPSCounter fpsc;
  private:
    App();
    FPSmanager fpsm;
    std::thread physicsThread;
};


# define GET_App App::Instance()