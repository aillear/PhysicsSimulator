#pragma once

#include "vector2.h"
#include <SDL_render.h>
#include <SDL_video.h>
#include <string>

struct DrawCommand {

};

struct Camera {
    // pos in world
    Vector2f position;
    float scale;
};

class RenderSystem {
  public:

    static RenderSystem &Instance();
    bool Init(int width = 1800, int height = 900,
              const std::string &windowName = "physics demo");
    void SetWindowSize(Vector2i size);
    void Render();

  private:
    RenderSystem();
    ~RenderSystem();
    Vector2i PosWorld2Screen(const Vector2f worldPos);
    Vector2f PosScreen2World(const Vector2i windowPos);

    SDL_Window *window;
    SDL_Renderer *renderer;
    Vector2i windowSize;
    Camera camera;
};

#define GET_RenderSystem RenderSystem::Instance()
#define WORLD2SCREEN(pos) RenderSystem::Instance().PosWorld2Screen(pos)
#define SCREEN2WORLD(pos) RenderSystem::Instance().PosScreen2World(pos)