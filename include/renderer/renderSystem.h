#pragma once

#include <SDL_render.h>
#include <SDL_video.h>
class RenderSystem {
  public:
    static RenderSystem& Instance();
    
  private:
    bool Init();

    
    SDL_Window* window;
    SDL_Renderer* renderer;
};