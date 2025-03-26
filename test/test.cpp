#include <SDL.h>
#include "vector2.h"
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int GRID_SIZE = 50;
const int MIN_GRID_SIZE = 10;
const int MAX_GRID_SIZE = 200;

int main(int argc, char *argv[]) {
    Vector2<float> a(1.2, 2.3);
    a *= 3;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "Advanced Grid System", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 视口系统
    struct {
        int x = WINDOW_WIDTH / 2;  // 原点X坐标
        int y = WINDOW_HEIGHT / 2; // 原点Y坐标
        float scale = 1.0f;        // 缩放系数
    } viewport;

    bool isDragging = false;
    SDL_Point dragStart;
    SDL_Point viewportStart;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    WINDOW_WIDTH = event.window.data1;
                    WINDOW_HEIGHT = event.window.data2;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                    dragStart = {event.button.x, event.button.y};
                    viewportStart = {viewport.x, viewport.y};
                }
                break;

            case SDL_MOUSEMOTION:
                if (isDragging) {
                    viewport.x =
                        viewportStart.x + (event.motion.x - dragStart.x);
                    viewport.y =
                        viewportStart.y + (event.motion.y - dragStart.y);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                }
                break;

            case SDL_MOUSEWHEEL: {
                // 获取鼠标在窗口中的位置
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // 计算缩放前的相对坐标
                float worldX = (mouseX - viewport.x) / viewport.scale;
                float worldY = (mouseY - viewport.y) / viewport.scale;

                // 更新缩放系数
                if (event.wheel.y > 0) {
                    viewport.scale *= 1.1f;
                    GRID_SIZE = static_cast<int>(GRID_SIZE * 1.1f);
                } else if (event.wheel.y < 0) {
                    viewport.scale /= 1.1f;
                    GRID_SIZE = static_cast<int>(GRID_SIZE / 1.1f);
                }

                // 限制网格尺寸
                GRID_SIZE = SDL_clamp(GRID_SIZE, MIN_GRID_SIZE, MAX_GRID_SIZE);

                // 调整视口位置保持鼠标位置稳定
                viewport.x = mouseX - worldX * viewport.scale;
                viewport.y = mouseY - worldY * viewport.scale;
                break;
            }
            }
        }

        // 清空屏幕
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // 计算有效网格尺寸（考虑缩放）
        int effectiveGridSize = static_cast<int>(GRID_SIZE * viewport.scale);

        // 优化绘制范围
        int startX = (-viewport.x) / effectiveGridSize - 2;
        int endX = (WINDOW_WIDTH - viewport.x) / effectiveGridSize + 2;
        int startY = (-viewport.y) / effectiveGridSize - 2;
        int endY = (WINDOW_HEIGHT - viewport.y) / effectiveGridSize + 2;

        // 绘制网格线
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (int x = startX; x < endX; ++x) {
            int screenX = viewport.x + x * effectiveGridSize;
            SDL_RenderDrawLine(renderer, screenX, 0, screenX, WINDOW_HEIGHT);
        }
        for (int y = startY; y < endY; ++y) {
            int screenY = viewport.y + y * effectiveGridSize;
            SDL_RenderDrawLine(renderer, 0, screenY, WINDOW_WIDTH, screenY);
        }

        // 绘制坐标轴
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 0, viewport.y, WINDOW_WIDTH,
                           viewport.y); // X轴
        SDL_RenderDrawLine(renderer, viewport.x, 0, viewport.x,
                           WINDOW_HEIGHT); // Y轴

        // 绘制刻度线
        const int TICK_LENGTH = 5;
        for (int x = startX; x < endX; ++x) {
            int screenX = viewport.x + x * effectiveGridSize;
            SDL_RenderDrawLine(renderer, screenX, viewport.y - TICK_LENGTH,
                               screenX, viewport.y + TICK_LENGTH);
        }
        for (int y = startY; y < endY; ++y) {
            int screenY = viewport.y + y * effectiveGridSize;
            SDL_RenderDrawLine(renderer, viewport.x - TICK_LENGTH, screenY,
                               viewport.x + TICK_LENGTH, screenY);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}