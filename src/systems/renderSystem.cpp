#include "renderSystem.h"
#include "eventSystem.h"
#include "logger.h"
#include "vector2.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_video.h>
#include <utility>

// singleton
RenderSystem &RenderSystem::Instance() {
    static RenderSystem instance;
    return instance;
}

RenderSystem::RenderSystem()
    : window(nullptr), renderer(nullptr), windowSize(0, 0),
      camera({0.0f, 0.0f}, 1) {
        UIdrawCommands.reserve(1000); // reserve 1000 draw commands
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
bool RenderSystem::Init(int width, int height, const std::string &windowName) {
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        F_LOG_ERROR("Fail to init SDL: {}", SDL_GetError());
        return false;
    }
    LOG_INFO("SDL initialized");

    // create window and renderer
    window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        F_LOG_ERROR("Fail to create SDL window: {}", SDL_GetError());
        return false;
    }
    F_LOG_INFO("SDL window {} created successfully.", windowName);
    windowSize = {width, height};
    camera.position = {0.5f * windowSize.x, windowSize.y * 0.5f};
    F_LOG_INFO("window size: {}.", windowSize);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        F_LOG_ERROR("Fail to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    LOG_INFO("SDL renderer created successfully.");

    // add window resize event listener
    GET_EventSystem.AddEventListener(SDL_WINDOWEVENT, [this](SDL_Event &event) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            this->SetWindowSize({event.window.data1, event.window.data2});
            this->camera.position = {0.5f * windowSize.x, windowSize.y * 0.5f};
        }
    });
    LOG_INFO("Add window resize event listener successfully.");
    return true;
}

void RenderSystem::SetWindowSize(Vector2i size) {
    windowSize = size;
    F_LOG_INFO("Resize window to: {}.", windowSize);
}

Vector2i RenderSystem::PosWorld2Screen(Vector2f worldPos) {
    Vector2f offset(camera.scale * windowSize.x * 0.5, camera.scale * windowSize.y * 0.5);
    Vector2f temp = worldPos - camera.position + offset;
    return Vector2i(temp.x, temp.y);
}

Vector2f RenderSystem::PosScreen2World(Vector2i windowPos) {
    Vector2f offset(-0.5 * camera.scale * windowSize.x,
                    -0.5 * camera.scale * windowSize.y);
    return camera.position + offset + Vector2f(windowPos.x, windowPos.y);
}

void RenderSystem::Render() {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
        // case DrawCommand::ShapeType::CIRCLE:
        //     DrawCircle(cmd);
        //     break;
        // case DrawCommand::ShapeType::TEXT:
        //     DrawText(cmd);
        //     break;
        default:
            break;
        }
    }
    UIdrawCommands.clear();
    SDL_RenderPresent(renderer);
}

void RenderSystem::AddUIDrawCommand(DrawCommand&& cmd) {
    UIdrawCommands.emplace_back(std::forward<DrawCommand>(cmd));
}





// all are untested.

void RenderSystem::DrawLine(DrawCommand &cmd) {
    Vector2i start = PosWorld2Screen(cmd.line.start);
    Vector2i end = PosWorld2Screen(cmd.line.end);
    SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b,
                           cmd.color.a);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

/**
 * @brief unstable method. do not use it!!!
 *
 * @param cmd drawCommand
 */
void RenderSystem::DrawTriangle(DrawCommand &cmd) {
    Vector2i p1 = PosWorld2Screen(cmd.triangle.p1);
    Vector2i p2 = PosWorld2Screen(cmd.triangle.p2);
    Vector2i p3 = PosWorld2Screen(cmd.triangle.p3);
    SDL_SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b,
                           cmd.color.a);
    SDL_Point points[4] = {
        {p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}, {p1.x, p1.y}};
    SDL_RenderDrawLines(renderer, points, 4);
}

/**
 * @brief Draw rectangle. if isFilled is true, draw a filled rectangle.
 *
 * @param cmd
 */
void RenderSystem::DrawRect(DrawCommand &cmd) {
    Vector2i topLeft = PosWorld2Screen(cmd.rect.topLeft);
    Vector2i buttomRight = PosWorld2Screen(cmd.rect.buttomRigt);

    if (cmd.isFilled)
        boxRGBA(renderer, topLeft.x, topLeft.y, buttomRight.x, buttomRight.y,
                cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);

    else
        rectangleRGBA(renderer, topLeft.x, topLeft.y, buttomRight.x,
                      buttomRight.y, cmd.color.r, cmd.color.g, cmd.color.b,
                      cmd.color.a);
}