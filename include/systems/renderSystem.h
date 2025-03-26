# pragma once

#include "vector2.h"
#include <string>
#include <SDL_render.h>
#include <SDL_video.h>
class RenderSystem {
  public:
    static RenderSystem& Instance();
    bool Init(int width=1800, int height=900, const std::string& windowName="physics demo");
    void SetWindowSize(Vector2i size);
    void Render();


  private:

    RenderSystem();
    ~RenderSystem();
    SDL_Window* window;
    SDL_Renderer* renderer;
    Vector2i windowSize;
};


# define GET_RenderSystem RenderSystem::Instance()