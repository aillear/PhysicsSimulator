#pragma once

#include "conversion.h"
#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <camera.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/ext/vector_int2.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>

struct GlmRect { glm::vec2 p1, p2; };
struct GlmCircle { glm::vec2 center; float radius; };
enum class ShapeType { POLYGON = 0, TEXT, LINE, RECT, CIRCLE };
struct BasicCommand {
    union {
        GlmRect rect;
        GlmCircle circle;
     };
    SDL_FColor color;
};

/**
 * @brief ComplexCommand is a union of SDL_Vertex and std::string.
 *  aabb is the bounding box of the command.
 *  The SDL_Vertex is used to draw the polygon.
 *  when it comes to be string, aabb is the size of the text.
 */
struct ComplexCommand {
    std::variant<
        std::vector<SDL_Vertex>,
        std::shared_ptr<TTF_Text>
    > data;
    SDL_FRect aabb;
    SDL_FColor color;  // ONLY for text


    /**
     * @brief Get the Vertexs object
     *        this function is unsafe.
     * 
     * @return std::vector<SDL_Vertex>& 
     */
    std::vector<SDL_Vertex> &GetVertexs() {
        return std::get<std::vector<SDL_Vertex>>(data);
    }

    /**
     * @brief Get the Text object
     *        this function is unsafe. 
     *
     * @return std::string& 
     */
     std::shared_ptr<TTF_Text> &GetText() {
        return std::get<std::shared_ptr<TTF_Text>>(data);
    }

    void SetFColor(SDL_FColor color) {
        this->color = color;
    }

    void SetColor(SDL_Color color) {
        this->color = ToFColor(color);
    }

    void SetAABB(SDL_FRect aabb) {
        this->aabb = aabb;
    }

    void SetTextPos(glm::vec2 pos) {
        aabb.x = pos.x;
        aabb.y = pos.y;
    }
};

struct DrawCommand {
    bool UIMode_;
    ShapeType shapeType;
    std::variant<BasicCommand, ComplexCommand> cmd;

    DrawCommand(ShapeType type, bool UIMode) : shapeType(type), UIMode_(UIMode) {
        switch (type) {
            case ShapeType::POLYGON: {
                auto cmdt = ComplexCommand();
                cmdt.data = std::vector<SDL_Vertex>();
                cmdt.aabb = {0, 0, 0, 0};
                cmd = cmdt;
                break;
            }
            case ShapeType::TEXT: {
                auto cmdt = ComplexCommand();
                cmdt.data = nullptr;
                cmdt.aabb = {0, 0, 0, 0};
                cmd = cmdt;
                break;
            }
            default:
                cmd = BasicCommand{};
                break;
        }
    }

    /**
     * @brief Get the Base object
     *       this function is unsafe.
     * 
     * @return BasicCommand& 
     */
    BasicCommand& GetBase() {
        return std::get<BasicCommand>(cmd);
    }

    /**
     * @brief Get the Complex object
     *       this function is unsafe.
     * 
     * @return ComplexCommand& 
     */
    ComplexCommand& GetComplex() {
        return std::get<ComplexCommand>(cmd);
    }
};

/**
 * @brief struct for initialization of RenderSyste
 * @param vertexBufferSize: size of the vertex buffer
 * @param fontSize: size of the font
 * @param windowSize: size of the window
 * @param bgColor: background color of the window
 * @param fontName: name of the font file
 * @param windowName: name of the window
 */
struct RenderSystemIniter {
    int vertexBufferSize = 1'000'000;
    float fontSize = 16.0f;
    glm::vec2 windowSize = {800, 600};
    SDL_Color bgColor = {50, 56, 66, 255};
    std::string fontName = "YeHei.ttf";
    std::string windowName = "Physics Simulator";
};

class RenderSystem {
  public:

    static RenderSystem &Instance();
    bool Init(const RenderSystemIniter &initer);
    glm::vec2 GetWindowSize() const { return halfWindowSize * 2.0f; }
    glm::vec2 GetWindowCenter() const { return halfWindowSize; }
    void SetWindowSize(glm::vec2 size);
    float GetFontSize() const { return fontSize; }
    void SetFontSize(float size);
    std::shared_ptr<TTF_Text> CreateText(const std::string& text, size_t size = 0);
    void AddUIDrawCommand(DrawCommand&& cmd);
    void Render();
    
  private:  
    RenderSystem();
    ~RenderSystem();
    SDL_FPoint PosWorld2Screen(const glm::vec2 worldPos);
    glm::vec2 PosScreen2World(const glm::vec2 windowPos);

    void HandleUIDrawCommand();
    
    void LineCommand(DrawCommand &cmd);
    void RectCommand(DrawCommand &cmd);
    void CircleCommand(DrawCommand &cmd);
    void PolygonCommand(DrawCommand &cmd);
    void TextCommand(DrawCommand &cmd);

    int vertexBufferSize;
    int indicesSize;
    int maxVertexBufferSize;
    int maxIndicesSize;
    float fontSize;
    glm::vec2 halfWindowSize;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_TextEngine *textEngine;
    SDL_Color backgroundColor;
    Camera camera;
    std::vector<DrawCommand> UIdrawCommands;
    std::unique_ptr<SDL_Vertex[]> vertexBuffer;
    std::unique_ptr<int[]> indices;
    
};

#define GET_RenderSystem RenderSystem::Instance()
#define WORLD2SCREEN(pos) RenderSystem::Instance().PosWorld2Screen(pos)
#define SCREEN2WORLD(pos) RenderSystem::Instance().PosScreen2World(pos)

template <int LOD>
struct CircleLodCache {
    static const std::array<std::pair<float, float>, 64>& getVertices() {
        static const auto cache = []() {
            std::array<std::pair<float, float>, 64> arr{};
            float angle_step = 2 * M_PI / LOD;
            for (int i = 0; i < LOD; ++i) {
                float theta = i * angle_step;
                arr[i] = {std::cos(theta), std::sin(theta)};
            }
            return arr;
        }();
        return cache;
    }
};