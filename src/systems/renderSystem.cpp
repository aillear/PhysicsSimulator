#include "renderSystem.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "conversion.h"
#include "eventSystem.h"
#include "logger.h"
#include "pathMgr.h"
#include "renderBufferMgr.h"
#include <SDL3/SDL.h>
#include <cstddef>
#include <cstring>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/geometric.hpp>
#include <memory>
#include <string>
#include <vector>

// lod level cache for circle
template struct CircleLodCache<8>;
template struct CircleLodCache<16>;
template struct CircleLodCache<24>;
template struct CircleLodCache<32>;
template struct CircleLodCache<40>;
template struct CircleLodCache<48>;

constexpr static int CIRCLE_LOD[] = {8, 16, 24, 32, 40, 48};
#define MAX_LOD_LEVEL CIRCLE_LOD[5]
const static std::vector<std::array<std::pair<float, float>, 64>>
    SIN_COS_CACHE = {
        CircleLodCache<8>::getVertices(),  CircleLodCache<16>::getVertices(),
        CircleLodCache<24>::getVertices(), CircleLodCache<32>::getVertices(),
        CircleLodCache<40>::getVertices(), CircleLodCache<48>::getVertices(),
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

void RenderSystem::Destroy() {
    TTF_DestroyRendererTextEngine(textEngine);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    LOG_INFO("Render system destroyed.");
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
bool RenderSystem::Init(const RenderSystemIniter &initer) {
    LOG_INFO("Render system initializing...");

    // init SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        F_LOG_ERROR("Fail to init SDL: {}", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        F_LOG_ERROR("Fail to init SDL_ttf: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }
    // create window and renderer
    window = SDL_CreateWindow(initer.windowName.c_str(), initer.windowSize.x,
                              initer.windowSize.y, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        F_LOG_ERROR("Fail to create SDL window: {}", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // SDL_GL_SetSwapInterval(0);
    halfWindowSize = 0.5f * initer.windowSize;
    F_LOG_INFO("Window size :{}", initer.windowSize);
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        F_LOG_ERROR("Fail to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // set background color
    backgroundColor = initer.bgColor;

    fontSize = initer.fontSize;
    font = TTF_OpenFont(
        GET_PATH("assets", "fonts", initer.fontName).string().c_str(),
        initer.fontSize);

    if (!font) {
        F_LOG_ERROR("Fail to load font: {}", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }
    textEngine = TTF_CreateRendererTextEngine(renderer);
    if (!textEngine) {
        F_LOG_ERROR("Fail to create text engine: {}", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // add window resize event listener
    camera.SetPosition(0.5f * initer.windowSize);
    camera.Init();
    GET_EventSystem.AddEventListener(
        SDL_EVENT_WINDOW_RESIZED, [this](SDL_Event &event) {
            glm::vec2 oldCenter = halfWindowSize;
            this->SetWindowSize({event.window.data1, event.window.data2});
            this->camera.SetPosition(camera.GetPosition() + halfWindowSize -
                                     oldCenter);
        });

    // set buffer size
    if (initer.vertexBufferSize <= 0) {
        F_LOG_ERROR("Vertex buffer size must be greater than 0, current is {}.",
                    initer.vertexBufferSize);
        TTF_DestroyRendererTextEngine(textEngine);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();

        return false;
    } else if (initer.vertexBufferSize < 10'000) {
        F_LOG_WARNING(
            "Vertex buffer size {} is too small. may cause some problem.",
            initer.vertexBufferSize);
    }
    maxVertexBufferSize = initer.vertexBufferSize;
    maxIndicesSize = initer.vertexBufferSize * 3; // 3 indices for each vertex
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

void RenderSystem::SetFontSize(float size) {
    if (size <= 0) {
        F_LOG_ERROR("Font size must be greater than 0, current is {}.", size);
        return;
    } else if (size < 8) {
        F_LOG_WARNING("Font size {} is too small. may cause some problem.",
                      size);
    } else if (size == fontSize) {
        return;
    }
    fontSize = size;
    TTF_SetFontSize(font, fontSize);
}

std::shared_ptr<TTF_Text> RenderSystem::CreateText(const std::string &text,
                                                   size_t size) {
    return std::shared_ptr<TTF_Text>(
        TTF_CreateText(textEngine, font, text.c_str(), size), TTF_DestroyText);
}

SDL_FPoint RenderSystem::PosWorld2Screen(glm::vec2 worldPos) {
    auto temp =
        (worldPos - camera.GetPosition()) * camera.getZoom() + halfWindowSize;
    return {temp.x, temp.y};
}

glm::vec2 RenderSystem::PosScreen2World(glm::vec2 screenPos) {
    return (screenPos - halfWindowSize) * camera.getZoomR() +
           camera.GetPosition();
}

void RenderSystem::Render() {

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                           backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    HandlePhysicsDrawCommand();
    HandleUIDrawCommand();

    SDL_RenderPresent(renderer);
}

void RenderSystem::AddUIDrawCommand(DrawCommand &&cmd) {
    UIdrawCommands.emplace_back(std::forward<DrawCommand>(cmd));
}

void RenderSystem::HandlePhysicsDrawCommand() {
    GET_Buffer.Prepare();
    auto &cmds = GET_Buffer.GetConsumeBuffer();
    indicesSize = 0;
    vertexBufferSize = 0;

    for (auto &cmd : cmds) {
        switch (cmd.shapeType) {
        case ShapeType::HOLLOW_POL:
            PolygonHollowCommand(cmd);
            break;
        case ShapeType::POLYGON:
            PolygonCommand(cmd);
            break;
        case ShapeType::TEXT:
            TextCommand(cmd);
            break;
        case ShapeType::LINE:
            LineCommand(cmd);
            break;
        case ShapeType::RECT:
            RectCommand(cmd);
            break;
        case ShapeType::HOLLOW_RECT:
            RectHollowCommand(cmd);
            break;
        case ShapeType::CIRCLE:
            CircleCommand(cmd);
            break;
        case ShapeType::HOLLOW_CIRCLE:
            CircleHollowCommand(cmd);
            break;
        }
    }
    if (vertexBufferSize > maxVertexBufferSize ||
        indicesSize > maxIndicesSize) {
        F_LOG_ERROR(
            "vertex buffer or indices buffer overflow, {} > {} || {} > {}",
            vertexBufferSize, maxVertexBufferSize, indicesSize, maxIndicesSize);
    }
    // check if the vertex buffer is empty
    if (vertexBufferSize > 0) {
        SDL_RenderGeometry(renderer, nullptr, vertexBuffer.get(),
                           vertexBufferSize, indices.get(), indicesSize);
    }
}

void RenderSystem::HandleUIDrawCommand() {
    indicesSize = 0;
    vertexBufferSize = 0;
    for (auto &cmd : UIdrawCommands) {
        switch (cmd.shapeType) {
        case ShapeType::HOLLOW_POL:
            PolygonHollowCommand(cmd);
            break;
        case ShapeType::POLYGON:
            PolygonCommand(cmd);
            break;
        case ShapeType::TEXT:
            TextCommand(cmd);
            break;
        case ShapeType::LINE:
            LineCommand(cmd);
            break;
        case ShapeType::RECT:
            RectCommand(cmd);
            break;
        case ShapeType::HOLLOW_RECT:
            RectHollowCommand(cmd);
            break;
        case ShapeType::CIRCLE:
            CircleCommand(cmd);
            break;
        case ShapeType::HOLLOW_CIRCLE:
            CircleHollowCommand(cmd);
            break;
        }
    }
    UIdrawCommands.clear();
    // check if the vertex buffer and indices buffer overflow
    if (vertexBufferSize > maxVertexBufferSize ||
        indicesSize > maxIndicesSize) {
        F_LOG_ERROR(
            "vertex buffer or indices buffer overflow, {} > {} || {} > {}",
            vertexBufferSize, maxVertexBufferSize, indicesSize, maxIndicesSize);
    }
    // check if the vertex buffer is empty
    if (vertexBufferSize > 0) {
        SDL_RenderGeometry(renderer, nullptr, vertexBuffer.get(),
                           vertexBufferSize, indices.get(), indicesSize);
    }
}

void RenderSystem::LineCommand(DrawCommand &cmd) {
    int vertexBegin = vertexBufferSize;
    SDL_Vertex *buffer = vertexBuffer.get();
    auto &command = cmd.GetBase();
    SDL_FColor color = command.color;
    SDL_FPoint p[4] = {};
    glm::vec2 dir = command.rect.p2 - command.rect.p1;
    dir = {dir.y, -dir.x};
    dir = glm::normalize(dir) * cmd.halfLineWidth;
    p[2] = ToFPoint(dir);

    // if not UI mode, convert the position to screen space
    if (cmd.UIMode_ == false) {
        p[0] = PosWorld2Screen(command.rect.p1) + p[2];
        p[1] = PosWorld2Screen(command.rect.p2) + p[2];
    } else {
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

void RenderSystem::LineCommand(SDL_Vertex &p1, SDL_Vertex &p2,
                               float halfLineWidth) {
    int vertexBegin = vertexBufferSize;
    SDL_Vertex *buffer = vertexBuffer.get();

    glm::vec2 dir = ToGlmVec2(p1.position - p2.position);
    dir = {dir.y, -dir.x};
    dir = glm::normalize(dir) * halfLineWidth;
    SDL_FPoint dirSDL = ToFPoint(dir);
    buffer[vertexBufferSize] = p1;
    buffer[vertexBufferSize++].position += dirSDL;

    buffer[vertexBufferSize] = p1;
    buffer[vertexBufferSize++].position -= dirSDL;

    buffer[vertexBufferSize] = p2;
    buffer[vertexBufferSize++].position += dirSDL;

    buffer[vertexBufferSize] = p2;
    buffer[vertexBufferSize++].position -= dirSDL;

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
    auto &command = cmd.GetBase();
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

void RenderSystem::RectHollowCommand(DrawCommand &cmd) {
    auto &command = cmd.GetBase();
    SDL_FColor color = command.color;
    SDL_FPoint p1, p2;

    if (cmd.UIMode_ == false) {
        p1 = PosWorld2Screen(command.rect.p1);
        p2 = PosWorld2Screen(command.rect.p2);
    } else {
        p1 = ToFPoint(command.rect.p1);
        p2 = ToFPoint(command.rect.p2);
    }

    SDL_Vertex v[4];
    for (int i = 0; i < 4; i++) {
        v[i].color = color;
        v[i].tex_coord = {0, 0};
    }

    v[0].position = p1;
    v[1].position = {p1.x, p2.y};
    v[2].position = p2;
    v[3].position = {p2.x, p1.y};

    LineCommand(v[0], v[1], cmd.halfLineWidth);
    LineCommand(v[1], v[2], cmd.halfLineWidth);
    LineCommand(v[2], v[3], cmd.halfLineWidth);
    LineCommand(v[3], v[0], cmd.halfLineWidth);
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
    if (radius <= 10)
        lodLevel = 0;
    else if (radius <= 15)
        lodLevel = 1;
    else if (radius <= 25)
        lodLevel = 2;
    else if (radius <= 50)
        lodLevel = 3;
    else if (radius <= 100)
        lodLevel = 4;
    else
        lodLevel = 5;

    const auto &cache = SIN_COS_CACHE[lodLevel];
    lodLevel = CIRCLE_LOD[lodLevel];

    int vertexBegin = vertexBufferSize;
    SDL_Vertex *buffer = vertexBuffer.get();
    SDL_FColor color = cmd.GetBase().color;

    for (int i = 0; i < lodLevel; i++) {
        buffer[vertexBufferSize].position =
            center +
            SDL_FPoint{radius * cache[i].first, radius * cache[i].second};
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

void RenderSystem::CircleHollowCommand(DrawCommand &cmd) {
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
    if (radius <= 10)
        lodLevel = 0;
    else if (radius <= 15)
        lodLevel = 1;
    else if (radius <= 25)
        lodLevel = 2;
    else if (radius <= 50)
        lodLevel = 3;
    else if (radius <= 100)
        lodLevel = 4;
    else
        lodLevel = 5;

    const auto &cache = SIN_COS_CACHE[lodLevel];
    lodLevel = CIRCLE_LOD[lodLevel];

    SDL_FColor color = cmd.GetBase().color;

    SDL_Vertex v[MAX_LOD_LEVEL];
    v[0].color = color;
    v[0].position =
        center + SDL_FPoint{radius * cache[0].first, radius * cache[0].second};
    v[0].tex_coord = {0, 0};
    for (int i = 1; i < lodLevel; i++) {
        v[i].color = color;
        v[i].tex_coord = {0, 0};
        v[i].position = center + SDL_FPoint{radius * cache[i].first,
                                            radius * cache[i].second};
        LineCommand(v[i - 1], v[i], cmd.halfLineWidth);
    }
    LineCommand(v[lodLevel - 1], v[0], cmd.halfLineWidth);
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
        for (auto &v : vertexs) {
            auto t = PosWorld2Screen(ToGlmVec2(v.position));
            if (t.x >= 300) {
                int a;
            }
            v.position = t;
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

void RenderSystem::PolygonHollowCommand(DrawCommand &cmd) {
    auto &vertexs = cmd.GetComplex().GetVertexs();
    int n = vertexs.size();
    if (n < 3) {
        F_LOG_WARNING("Polygon must have at least 3 vertices, but {} given.",
                      n);
        return;
    }

    // if not UI mode, convert the position to screen space
    if (cmd.UIMode_ == false) {
        for (auto &v : vertexs) {
            v.position = PosWorld2Screen(ToGlmVec2(v.position));
        }
    }

    for (int i = 1; i < n; i++) {
        LineCommand(vertexs[i - 1], vertexs[i], cmd.halfLineWidth);
    }
    LineCommand(vertexs[n - 1], vertexs[0], cmd.halfLineWidth);
}

void RenderSystem::TextCommand(DrawCommand &cmd) {
    auto text = cmd.GetComplex().GetText().get();
    SDL_FColor color = cmd.GetComplex().color;

    SDL_FPoint pos;
    if (cmd.UIMode_ == false) {
        pos =
            PosWorld2Screen({cmd.GetComplex().aabb.x, cmd.GetComplex().aabb.y});
    } else {
        pos = ToFPoint({cmd.GetComplex().aabb.x, cmd.GetComplex().aabb.y});
    }

    TTF_SetTextColorFloat(text, color.r, color.g, color.b, color.a);

    // need to render submit vertex first so that the text can be display in
    // right order.
    if (vertexBufferSize != 0) {
        SDL_RenderGeometry(renderer, nullptr, vertexBuffer.get(),
                           vertexBufferSize, indices.get(), indicesSize);

        vertexBufferSize = 0;
        indicesSize = 0;
    }
    TTF_DrawRendererText(text, pos.x, pos.y);
}
