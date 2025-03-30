#pragma once

#include "vector2.h"
#include "transform.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
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
        struct {glm::vec2 start, end;} line;
        struct {glm::vec2 p1, p2, p3;} triangle;
        struct {glm::vec2 topLeft, buttomRigt;} rect;
        struct {float radius;} circle;
    };
    std::shared_ptr<std::string> text;
    SDL_Color color;
};

struct Camera {
    // pos in world
    glm::vec2 position;
    float scale;
};

class RenderSystem {
  public:

    static RenderSystem &Instance();
    bool Init(int width = 1800, int height = 900,
              const std::string &windowName = "physics demo");
    void SetWindowSize(glm::ivec2 size);
    void AddUIDrawCommand(DrawCommand&& cmd);
    void Render();
    
  private:
    RenderSystem();
    ~RenderSystem();
    glm::vec2 PosWorld2Screen(const glm::vec2 worldPos);
    glm::vec2 PosScreen2World(const glm::vec2 windowPos);

    inline void DrawLine(DrawCommand& cmd);
    inline void DrawTriangle(DrawCommand& cmd);
    inline void DrawRect(DrawCommand& cmd);
    inline void DrawCircle(DrawCommand& cmd);
    inline void DrawText(DrawCommand& cmd);

    SDL_Window *window;
    SDL_Renderer *renderer;
    glm::ivec2 windowSize;
    Camera camera;
    SDL_Color backgroundColor;
    std::vector<DrawCommand> UIdrawCommands;
    
};

#define GET_RenderSystem RenderSystem::Instance()
#define WORLD2SCREEN(pos) RenderSystem::Instance().PosWorld2Screen(pos)
#define SCREEN2WORLD(pos) RenderSystem::Instance().PosScreen2World(pos)