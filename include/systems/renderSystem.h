#pragma once

#include "transform.h"
#include <glm/detail/qualifier.hpp>
#include <memory>
#include <string>
#include <vector>
#include <camera.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float2.hpp>

/**
 * @brief
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
    int layer;
    enum class ShapeType { LINE, TRIANGLE, RECT, CIRCLE, TEXT } shapeType;
    
    union {
        struct {glm::vec2 start, end;} line;
        struct {glm::vec2 p1, p2, p3;} triangle;
        struct {glm::vec2 center, size;} rect;
        struct {glm::vec2 center; float radius;} circle;
    };
    std::shared_ptr<std::string> text;
    SDL_Color color;
};



class RenderSystem {
  public:

    static RenderSystem &Instance();
    bool Init(int vertexsBufferSize = 1'000'000, int width = 1800, int height = 900,
              const std::string &windowName = "physics demo");
    glm::vec2 GetWindowSize() const { return halfWindowSize * 2.0f; }
    glm::vec2 GetWindowCenter() const { return halfWindowSize; }
    void SetWindowSize(glm::vec2 size);
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
    
    int vertexBufferSize;
    int indicesSize;
    int maxVertexBufferSize;
    int maxIndicesSize;
    glm::vec2 halfWindowSize;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Color backgroundColor;
    Camera camera;
    std::vector<DrawCommand> UIdrawCommands;
    std::unique_ptr<SDL_Vertex[]> vertexBuffer;
    std::unique_ptr<int[]> indices;
    
};

#define GET_RenderSystem RenderSystem::Instance()
#define WORLD2SCREEN(pos) RenderSystem::Instance().PosWorld2Screen(pos)
#define SCREEN2WORLD(pos) RenderSystem::Instance().PosScreen2World(pos)