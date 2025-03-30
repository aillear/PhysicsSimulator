#include "vector2.h"
#include "logger.h"
#include "eventSystem.h"
#include "renderSystem.h"
#include <cstddef>
#include <glm/ext/vector_float2.hpp>
#include <utility>
#include <SDL3./SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3_gfxPrimitives.h>
#include <glm/ext/vector_int2.hpp>

// singleton
RenderSystem &RenderSystem::Instance() {
    static RenderSystem instance;
    return instance;
}

RenderSystem::RenderSystem()
    : window(nullptr), renderer(nullptr), halfWindowSize(0, 0),
      camera({0.0f, 0.0f}, 1) {
    UIdrawCommands.reserve(10000); // reserve 10000 draw commands
    ;
}

RenderSystem::~RenderSystem() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    LOG_INFO("SDL renderer and window has been destroied successfully.");
    SDL_Quit();
    LOG_INFO("SDL quit");
}

/**
 * @brief initialize SDL and create window and renderer.
 *
 * @param width width of the window
 * @param height height of the window
 * @param windowName window display name
 * @return true initialized successfully
 * @return false fail to initialize
 */
bool RenderSystem::Init(int vertexBufferSize, int width, int height, const std::string &windowName) {
    LOG_INFO("Render system initializing...");

    // init SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        F_LOG_ERROR("Fail to init SDL: {}", SDL_GetError());
        return false;
    }
    // create window and renderer
    window = SDL_CreateWindow(windowName.c_str(), width, height,
                              SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        F_LOG_ERROR("Fail to create SDL window: {}", SDL_GetError());
        return false;
    }
    halfWindowSize = {0.5f * width, 0.5f * height};
    F_LOG_INFO("Window size :{}", Vector2f(halfWindowSize));
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        F_LOG_ERROR("Fail to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // set background color
    backgroundColor = {44, 49, 60, 255};
    // add window resize event listener
    camera.setPosition(0.5f * halfWindowSize.x, 0.5f * halfWindowSize.y);
    camera.Init();
    GET_EventSystem.AddEventListener(
        SDL_EVENT_WINDOW_RESIZED, [this](SDL_Event &event) {
            glm::vec2 oldCenter = halfWindowSize;
            this->SetWindowSize({event.window.data1, event.window.data2});
            this->camera.setPosition(camera.getPosition() + halfWindowSize - oldCenter);
        });

    // set buffer size
    if (vertexBufferSize <= 0) {
        F_LOG_ERROR("Vertex buffer size must be greater than 0, current is {}.", vertexBufferSize);
        return false;
    }
    else if (vertexBufferSize < 10'000) {
        F_LOG_WARNING("Vertex buffer size {} is too small. may cause some problem.", vertexBufferSize);
    }
    maxVertexBufferSize = vertexBufferSize;
    maxIndicesSize = vertexBufferSize * 3; // 3 indices for each vertex
    vertexBuffer = std::make_unique<SDL_Vertex[]>(maxVertexBufferSize);
    indices = std::make_unique<int[]>(maxIndicesSize);
    F_LOG_INFO("Vertex buffer size: {}.", maxVertexBufferSize);
    F_LOG_INFO("Indices buffer size: {}.", maxIndicesSize);

    LOG_INFO("Render system initialized successfully.");
    return true;
}

void RenderSystem::SetWindowSize(glm::vec2 size) {
    halfWindowSize = 0.5f * size;
    F_LOG_INFO("Resize window to: {}.", (Vector2f)halfWindowSize * 2.0f);
}

glm::vec2 RenderSystem::PosWorld2Screen(glm::vec2 worldPos) {
    return (worldPos - camera.getPosition()) * camera.getZoom() + halfWindowSize;
}

glm::vec2 RenderSystem::PosScreen2World(glm::vec2 screenPos) {
    return (screenPos - halfWindowSize) * camera.getZoomR() + camera.getPosition();
}

void RenderSystem::Render() {

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                           backgroundColor.b, backgroundColor.a);

    SDL_RenderClear(renderer);

    for (auto &cmd : UIdrawCommands) {
        switch (cmd.shapeType) {
        case DrawCommand::ShapeType::LINE:
            DrawLine(cmd);
            break;
        case DrawCommand::ShapeType::TRIANGLE:
            DrawTriangle(cmd);
            break;
        case DrawCommand::ShapeType::RECT:
            DrawRect(cmd);
            break;
        default:
            break;
        }
    }
    UIdrawCommands.clear();
    SDL_RenderPresent(renderer);
}

void RenderSystem::AddUIDrawCommand(DrawCommand &&cmd) {
    UIdrawCommands.emplace_back(std::forward<DrawCommand>(cmd));
}

// all are untested.

void RenderSystem::DrawLine(DrawCommand &cmd) {
    glm::vec2 start = PosWorld2Screen(cmd.line.start);
    glm::vec2 end = PosWorld2Screen(cmd.line.end);
    SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b,
                           cmd.color.a);
    SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
}

/**
 * @brief unstable method. do not use it!!!
 *
 * @param cmd drawCommand
 */
void RenderSystem::DrawTriangle(DrawCommand &cmd) {
    glm::vec2 p1 = PosWorld2Screen(cmd.triangle.p1);
    glm::vec2 p2 = PosWorld2Screen(cmd.triangle.p2);
    glm::vec2 p3 = PosWorld2Screen(cmd.triangle.p3);
    SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b,
                           cmd.color.a);
    SDL_FPoint points[4] = {
        {p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}, {p1.x, p1.y}};
    SDL_RenderLines(renderer, points, 4);
}

/**
 * @brief Draw rectangle. if isFilled is true, draw a filled rectangle.
 *
 * @param cmd
 */
void RenderSystem::DrawRect(DrawCommand &cmd) {
    glm::vec2 topLeft = PosWorld2Screen(cmd.rect.topLeft);
    glm::vec2 buttomRight = PosWorld2Screen(cmd.rect.buttomRigt);
    rectangleRGBA(renderer, topLeft.x, topLeft.y, buttomRight.x, buttomRight.y,
                  cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
}