#include "app.h"
#include "UIMgr.h"
#include "collisionMgr.h"
#include "configs.h"
#include "eventSystem.h"
#include "inputSystem.h"
#include "logger.h"
#include "object.h"
#include "pathMgr.h"
#include "physicsSystem.h"
#include "renderBufferMgr.h"
#include "renderSystem.h"
#include <SDL3_framerate.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/vec2.hpp>
#include <mutex>
#include <thread>

std::mutex physicsThreadMutex;

App &App::Instance() {
    static App instance;
    return instance;
}
App::App() : running(false) { ; }

void App::Init(int argc, char *argv[]) {
    // utils initialize
    GET_PathMgr.Init(); // PathMgr should be initialized first
#ifdef _DEBUG_MODE
    GET_Logger.Init(Logger::DEBUG, "app.log",
                    true); // when this is done, the logger will be usable
#else
    GET_Logger.Init(Logger::INFO, "app.log",
                    false); // when this is done, the logger will be usable
#endif
    GET_Logger.SetInstantFlush(true);
    // system initialize
    GET_EventSystem.Init();
    GET_Buffer.Init();
    GET_InputSystem.Init();
    RenderSystemIniter RenderSystemIniter;
    if (!GET_RenderSystem.Init(RenderSystemIniter)) {
        LOG_ERROR("Render system initialize failed.");
        this->Destroy();
        exit(1);
    }
    GET_PhysicsSystem.Init(20);
    GET_CollisionMgr.Init();
    GET_UIMgr.Init();
    GET_Configer.Init();
    SDL_initFramerate(&fpsm);
    SDL_setFramerate(&fpsm, 100);

    // add event listener
    // quit event
    GET_EventSystem.AddEventListener(
        SDL_EVENT_QUIT, [this](SDL_Event &event) { this->running = false; });

    SetRunning();
    LOG_INFO("App initialized.");
    LOG_INFO("Running...");
    for (BasicFunctionWrapper &callBack : customInit_)
        callBack();
    LOG_INFO("==========================");
#ifdef _DEBUG_MODE
    LOG_DEBUG("current is debug mode");
#endif
}

void App::Run() {

    for (auto &callBack : customInit_)
        callBack();

    physicsThreadMutex.lock();
    physicsThread = std::thread([]() {
        std::lock_guard<std::mutex> lock(physicsThreadMutex);
        GET_PhysicsSystem.SetRunning();
        GET_PhysicsSystem.UpdateWrapper();
    });
    physicsThreadMutex.unlock();

    SDL_Event slowUpdateEvent;
    slowUpdateEvent.type = USER_EVENT_SLOW_UPDATE;
    SDL_PushEvent(&slowUpdateEvent);

    while (running) {
        Update();
        for (auto &callBack : updateWrapper_)
            callBack();
    }
    physicsThread.join();
}

void App::Update() {
    fpsc.StartFrame();
    SDL_framerateDelay(&fpsm);

    timer_ += fpsc.GetLastFrameTime();
    if (timer_ >= 500) {
        timer_ -= 500;
        SDL_Event slowUpdateEvent;
        slowUpdateEvent.type = USER_EVENT_SLOW_UPDATE;
        SDL_PushEvent(&slowUpdateEvent);
    }

    GET_EventSystem.HandleEvent();
    GET_InputSystem.Update();
    GET_UIMgr.Update(fpsc.GetLastFrameTime());
    GET_RenderSystem.Render();
    fpsc.EndFrame();
}

void App::Destroy() {
    GET_InputSystem.Destroy();
    GET_UIMgr.Destroy();
    GET_PhysicsSystem.Destroy();
    GET_RenderSystem.Destroy();
    GET_Buffer.Destroy();
    GET_EventSystem.Destroy();
    GET_Logger.Destroy();
    GET_PathMgr.Destroy();
}
