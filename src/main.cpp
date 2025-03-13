#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2_framerate.h>
#include "logger.h"

#define SCREEN_WIDTH  1000
#define SCREEN_HEIGHT 800

int main(int argc, char* argv[]) {
    Logger::Instance().Init();
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    FPSmanager fpsmgr;
    
    // LOG_INFO("the program is begin to run.");
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
   //     LOG_ERROR("error in init.");
        return 1;
    }

    // 创建窗口和渲染器
    window = SDL_CreateWindow("SDL2_gfx Minimal Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!window || !renderer) {
    //    LOG_ERROR("error in building window or renderer.");
        SDL_Quit();
        return 1;
    }

    // 初始化帧率控制器
    SDL_initFramerate(&fpsmgr);
    SDL_setFramerate(&fpsmgr, 60);

    int quit = 0;
    float angle = 0.0f;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // 绘制多种基础图形（验证 SDL2_gfx 工作）
        filledCircleColor(renderer, 100, 100, 50, 0xFF0000FF);      // 红色实心圆
        aacircleColor(renderer, 200, 100, 80, 0xFF00FF00);          // 绿色抗锯齿圆环
        thickLineColor(renderer, 300, 100, 500, 300, 5, 0xFFFF0000);// 蓝色粗线
        roundedRectangleColor(renderer, 400, 400, 600, 500, 20, 0xFF00FFFF); // 黄色圆角矩形

        SDL_RenderPresent(renderer);
        
        SDL_framerateDelay(&fpsmgr);
        angle += 1.0f;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}