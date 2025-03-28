#pragma once

#include "vector2.h"
#include "transform.h"
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief
 * isFilled: filled or not
 * layer: render layer of the object
 * ShapeType: shape type
 * transform: transform of the object
 * line: line start and end
 * triangle: triangle three points
 * rect: rectangle top left and buttom right
 * circle: circle radius
 * text: text content
 * color: color of the object
 */
struct DrawCommand {
    bool isFilled;
    int layer;
    enum class ShapeType { LINE, TRIANGLE, RECT, CIRCLE, TEXT } shapeType;
    
    Transform transform;
    union {
        struct {Vector2f start, end;} line;
        struct {Vector2f p1, p2, p3;} triangle;
        struct {Vector2f topLeft, buttomRigt;} rect;
        struct {float radius;} circle;
    };
    std::shared_ptr<std::string> text;
    SDL_Color color;
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
    std::vector<DrawCommand> drawCommands;
  private:
    RenderSystem();
    ~RenderSystem();
    Vector2i PosWorld2Screen(const Vector2f worldPos);
    Vector2f PosScreen2World(const Vector2i windowPos);

    inline void DrawLine(DrawCommand& cmd);
    inline void DrawTriangle(DrawCommand& cmd);
    inline void DrawRect(DrawCommand& cmd);
    inline void DrawCircle(DrawCommand& cmd);
    inline void DrawText(DrawCommand& cmd);

    SDL_Window *window;
    SDL_Renderer *renderer;
    Vector2i windowSize;
    Camera camera;
    
};

#define GET_RenderSystem RenderSystem::Instance()
#define WORLD2SCREEN(pos) RenderSystem::Instance().PosWorld2Screen(pos)
#define SCREEN2WORLD(pos) RenderSystem::Instance().PosScreen2World(pos)