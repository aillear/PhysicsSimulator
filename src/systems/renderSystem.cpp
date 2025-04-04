#include "renderSystem.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "conversion.h"
#include "eventSystem.h"
#include "logger.h"
#include <SDL3./SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_gfxPrimitives.h>
#include <cstddef>
#include <cstring>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/geometric.hpp>
#include <utility>
#include <vector>


// lod level cache for circle
template struct CircleLodCache<8>;
template struct CircleLodCache<16>;
template struct CircleLodCache<24>;
template struct CircleLodCache<32>;
template struct CircleLodCache<40>;
template struct CircleLodCache<48>;

constexpr static int CIRCLE_LOD[] = {8, 16, 24, 32, 40, 48};
const static std::vector<std::array<std::pair<float, float>, 64>> SIN_COS_CACHE = {
    CircleLodCache<8>::getVertices(),
    CircleLodCache<16>::getVertices(),
    CircleLodCache<24>::getVertices(),
    CircleLodCache<32>::getVertices(),
    CircleLodCache<40>::getVertices(),
    CircleLodCache<48>::getVertices(),
};


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
bool RenderSystem::Init(int vertexBufferSize, int width, int height, SDL_Color bgColor,
                        const std::string &windowName) {
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
    // SDL_GL_SetSwapInterval(0);
    halfWindowSize = {0.5f * width, 0.5f * height};
    F_LOG_INFO("Window size :{}", halfWindowSize);
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        F_LOG_ERROR("Fail to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // set background color
    backgroundColor = bgColor;
    // add window resize event listener
    camera.setPosition(0.5f * halfWindowSize.x, 0.5f * halfWindowSize.y);
    camera.Init();
    GET_EventSystem.AddEventListener(
        SDL_EVENT_WINDOW_RESIZED, [this](SDL_Event &event) {
            glm::vec2 oldCenter = halfWindowSize;
            this->SetWindowSize({event.window.data1, event.window.data2});
            this->camera.setPosition(camera.getPosition() + halfWindowSize -
                                     oldCenter);
        });

    // set buffer size
    if (vertexBufferSize <= 0) {
        F_LOG_ERROR("Vertex buffer size must be greater than 0, current is {}.",
                    vertexBufferSize);
        return false;
    } else if (vertexBufferSize < 10'000) {
        F_LOG_WARNING(
            "Vertex buffer size {} is too small. may cause some problem.",
            vertexBufferSize);
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
    F_LOG_INFO("Resize window to: {}.", halfWindowSize * 2.0f);
}

SDL_FPoint RenderSystem::PosWorld2Screen(glm::vec2 worldPos) {
    auto temp =
        (worldPos - camera.getPosition()) * camera.getZoom() + halfWindowSize;
    return {temp.x, temp.y};
}

glm::vec2 RenderSystem::PosScreen2World(glm::vec2 screenPos) {
    return (screenPos - halfWindowSize) * camera.getZoomR() +
           camera.getPosition();
}

void RenderSystem::Render() {

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                           backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    HandleUIDrawCommand();

    SDL_RenderPresent(renderer);
}

void RenderSystem::AddUIDrawCommand(DrawCommand &&cmd) {
    UIdrawCommands.emplace_back(std::forward<DrawCommand>(cmd));
}

void RenderSystem::HandleUIDrawCommand() {
    indicesSize = 0;
    vertexBufferSize = 0;
    bool isLastText = false;
    for (auto &cmd : UIdrawCommands) {
        switch (cmd.shapeType) {
        case ShapeType::LINE:
            LineCommand(cmd);
            break;
        case ShapeType::RECT:
            RectCommand(cmd);
            break;
        case ShapeType::CIRCLE:
            CircleCommand(cmd);
            break;
        case ShapeType::POLYGON:
            PolygonCommand(cmd);
            break;
        case ShapeType::TEXT:
            TextCommand(cmd);
            isLastText = true;
            break;
        }
    }
    // check if the vertex buffer and indices buffer overflow
    if (vertexBufferSize > maxVertexBufferSize ||
        indicesSize > maxIndicesSize) {
        F_LOG_ERROR(
            "vertex buffer or indices buffer overflow, {} > {} || {} > {}",
            vertexBufferSize, maxVertexBufferSize, indicesSize, maxIndicesSize);
    }
    if (vertexBufferSize > 0) {
        SDL_RenderGeometry(renderer, nullptr, vertexBuffer.get(),
                           vertexBufferSize, indices.get(), indicesSize);
    }
    UIdrawCommands.clear();
}

void RenderSystem::LineCommand(DrawCommand &cmd) {
    int vertexBegin = vertexBufferSize;
    SDL_Vertex *buffer = vertexBuffer.get();
    auto& command = cmd.GetBase();
    SDL_FColor color = command.color;
    SDL_FPoint p[4] = {};
    glm::vec2 dir = command.rect.p2 - command.rect.p1;
    dir = {dir.y, -dir.x};
    dir = glm::normalize(dir);
    p[2] = ToFPoint(dir);

    // if not UI mode, convert the position to screen space
    if (cmd.UIMode_ == false) {
        p[0] = PosWorld2Screen(command.rect.p1) + p[2];
        p[1] = PosWorld2Screen(command.rect.p2) + p[2];
    } 
    else {
        p[0] = ToFPoint(command.rect.p1) + p[2];
        p[1] = ToFPoint(command.rect.p2) + p[2];
    }
    p[3] = p[1] - p[2];
    p[2] = p[0] - p[2];

    for (int i = 0; i < 4; i++) {
        buffer[vertexBufferSize].position = p[i];
        buffer[vertexBufferSize].color = color;
        buffer[vertexBufferSize].tex_coord = {0, 0};
        vertexBufferSize++;
    }

    indices[indicesSize++] = vertexBegin + 0;
    indices[indicesSize++] = vertexBegin + 1;
    indices[indicesSize++] = vertexBegin + 2;
    indices[indicesSize++] = vertexBegin + 1;
    indices[indicesSize++] = vertexBegin + 2;
    indices[indicesSize++] = vertexBegin + 3;
}

void RenderSystem::RectCommand(DrawCommand &cmd) {
    int vertexBegin = vertexBufferSize;
    SDL_Vertex *buffer = vertexBuffer.get();
    auto& command = cmd.GetBase();
    SDL_FColor color = command.color;
    SDL_FPoint p[4] = {};
    
    // if not UI mode, convert the position to screen space
    if (cmd.UIMode_ == false) {
        p[0] = PosWorld2Screen(command.rect.p1);
        p[1] = PosWorld2Screen(command.rect.p2);
    } else {
        p[0] = ToFPoint(command.rect.p1);
        p[1] = ToFPoint(command.rect.p2);
    }

    p[2] = {p[0].x, p[1].y};
    p[3] = {p[1].x, p[0].y};

    for (int i = 0; i < 4; i++) {
        buffer[vertexBufferSize].position = p[i];
        buffer[vertexBufferSize].color = color;
        buffer[vertexBufferSize].tex_coord = {0, 0};
        vertexBufferSize++;
    }

    indices[indicesSize++] = vertexBegin + 0;
    indices[indicesSize++] = vertexBegin + 2;
    indices[indicesSize++] = vertexBegin + 3;
    indices[indicesSize++] = vertexBegin + 2;
    indices[indicesSize++] = vertexBegin + 3;
    indices[indicesSize++] = vertexBegin + 1;
}

void RenderSystem::CircleCommand(DrawCommand &cmd) {
    // if not UI mode, convert the position and radius to screen space
    SDL_FPoint center;
    float radius;
    if (cmd.UIMode_ == false) {
        center = PosWorld2Screen(cmd.GetBase().circle.center);
        radius = cmd.GetBase().circle.radius * camera.getZoom();
    } else {
        center = ToFPoint(cmd.GetBase().circle.center);
        radius = cmd.GetBase().circle.radius;
    }

    if (radius < 0) {
        F_LOG_WARNING("Circle radius must be greater than 0, but {} given.",
                      radius);
        return;
    }

    // set lod level
    int lodLevel;
    if (radius <= 10) lodLevel = 0;
    else if (radius <= 15) lodLevel = 1;
    else if (radius <= 25) lodLevel = 2;
    else if (radius <= 50) lodLevel = 3;
    else if (radius <= 100) lodLevel = 4;
    else lodLevel = 5;

    const auto& cache = SIN_COS_CACHE[lodLevel];
    lodLevel = CIRCLE_LOD[lodLevel];

    int vertexBegin = vertexBufferSize;
    SDL_Vertex *buffer = vertexBuffer.get();
    SDL_FColor color = cmd.GetBase().color;

    for (int i = 0; i < lodLevel; i++) {
        buffer[vertexBufferSize].position = center + SDL_FPoint{radius*cache[i].first,
                                                      radius * cache[i].second};
        buffer[vertexBufferSize].color = color;
        buffer[vertexBufferSize].tex_coord = {0, 0};
        vertexBufferSize++;
    }

    lodLevel -= 2;
    for (int i = 0; i < lodLevel; i++) {
        indices[indicesSize++] = vertexBegin + 0;
        indices[indicesSize++] = vertexBegin + i + 1;
        indices[indicesSize++] = vertexBegin + i + 2;
    }
}

void RenderSystem::PolygonCommand(DrawCommand &cmd) {
    SDL_Vertex *buffer = vertexBuffer.get();
    auto &vertexs = cmd.GetComplex().GetVertexs();
    SDL_Vertex *p = vertexs.data();
    int n = vertexs.size();
    if (n < 3) {
        F_LOG_WARNING("Polygon must have at least 3 vertices, but {} given.",
                      n);
        return;
    }
    
    // if not UI mode, convert the position to screen space
    if (cmd.UIMode_ == false) {
        for (auto& v : vertexs) {
            v.position = PosWorld2Screen(ToGlmVec2(v.position));
        }
    }

    memcpy(buffer + vertexBufferSize, p, n * sizeof(SDL_Vertex));

    n -= 2;
    for (int i = 0; i < n; i++) {
        indices[indicesSize++] = vertexBufferSize;
        indices[indicesSize++] = vertexBufferSize + i + 1;
        indices[indicesSize++] = vertexBufferSize + i + 2;
    }
    vertexBufferSize += n + 2;
}

// TODO: implement text command
void RenderSystem::TextCommand(DrawCommand &cmd) {

}