#pragma once
#include "FPSCounter.h"
#include <SDL.h>
#include <SDL2_framerate.h>
class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void Run();
    bool running;
    FPSCounter fpsc;
  private:
    App();
    void Destory();
    FPSmanager fpsm;
    
};


# define GET_App App::Instance()