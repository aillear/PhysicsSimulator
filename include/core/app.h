#pragma once
#include "FPSCounter.h"
#include <SDL3/SDL.h>
#include <SDL3_framerate.h>
#include <thread>


class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void SetRunning() {running = true;};
    void Run();
    void Destroy();
    
    FPSCounter fpsc;
  private:
    App();

    bool running;
    FPSmanager fpsm;
    std::thread physicsThread;
};


# define GET_App App::Instance()