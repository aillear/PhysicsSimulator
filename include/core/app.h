#pragma once
#include "FPScounter.h"
#include <SDL.h>
#include <SDL2_framerate.h>
class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void Run();
    bool running;

  private:
    App();
    void Destory();
    FPSmanager fpsm;
    FPSCounter fpsc;
};


# define GET_App App::Instance()