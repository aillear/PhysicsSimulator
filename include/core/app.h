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
    static App &Instance();
    void Init(int argc, char *argv[]);
    void SetRunning() { running = true; };
    void Run();
    void Destroy();
    void AddCustomInit(BasicFunctionWrapper callBack) {
        customInit_.emplace_back(std::move(callBack));
    }
    void AddCustomUpdateSlow(BasicFunctionWrapper callBack) {
        slowUpdateWrapper_.emplace_back(std::move(callBack));
    }
    void AddCustomUpdate(BasicFunctionWrapper callBack) {
        updateWrapper_.emplace_back(std::move(callBack));
    }

    FPSCounter fpsc;

  private:
    App();
    void Update();

    float timer_;
    bool running;
    FPSmanager fpsm;
    std::thread physicsThread;

    std::vector<BasicFunctionWrapper> customInit_  ;
    std::vector<BasicFunctionWrapper> slowUpdateWrapper_;
    std::vector<BasicFunctionWrapper> updateWrapper_;
};

#define GET_App App::Instance()