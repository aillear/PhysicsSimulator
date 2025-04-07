#pragma once
#include "FPSCounter.h"
#include <SDL3/SDL.h>
#include <SDL3_framerate.h>
class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void Run();
    bool running;
    FPSCounter fpsc;
  private:
    App();
    void Destroy();
    FPSmanager fpsm;
    
};


# define GET_App App::Instance()