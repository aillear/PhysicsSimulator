#pragma once
#include "FPSCounter.h"
#include "object.h"
#include <SDL3/SDL.h>
#include <SDL3_framerate.h>
#include <thread>
#include <utility>
#include <vector>


class App {
  public:
    static App& Instance();
    void Init(int argc, char* argv[]);
    void SetRunning() {running = true;};
    void Run();
    void Destroy();
    void AddFuntionWhenInit(BasicFunctionWrapper callBack) {initFunctionWrapper_.emplace_back(std::move(callBack));}
    void AddFuntionBeforeRun(BasicFunctionWrapper callBack) {beforeRunFunctionWrapper_.emplace_back(std::move(callBack));}

    FPSCounter fpsc;
  private:
    App();

    bool running;
    FPSmanager fpsm;
    std::thread physicsThread;

    std::vector<BasicFunctionWrapper> initFunctionWrapper_;
    std::vector<BasicFunctionWrapper> beforeRunFunctionWrapper_;
};


# define GET_App App::Instance()