#include "camera.h"
#include "SDL3/SDL_mouse.h"
#include "eventSystem.h"
#include "renderSystem.h"
#include <glm/ext/vector_float2.hpp>

constexpr float MIN_ZOOM = 0.25f;
constexpr float MAX_ZOOM = 10.0f;

bool ZoomClamp(float& zoom, float factor) {
    factor *= zoom;
    if (factor < MIN_ZOOM || factor > MAX_ZOOM) return false;
    zoom = factor;
    return true;
}

void Camera::Init() {
    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_DOWN, [this](SDL_Event &event) {
            if (event.button.button == SDL_BUTTON_MIDDLE) {
                isDragging = true;
                lastPosition = {event.button.x, event.button.y};
            }
        });

    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_MOTION, [this](SDL_Event &event) {
            if (isDragging) {

                const glm::vec2 newMousePos = {event.motion.x, event.motion.y};
                position_ += (lastPosition - newMousePos) * zoomR * 0.01f;
                lastPosition = newMousePos;
            }
        });

    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_BUTTON_UP, [this](SDL_Event &event) {
            if (event.button.button == SDL_BUTTON_MIDDLE) {
                isDragging = false;
            }
        });

    GET_EventSystem.AddEventListener(
        SDL_EVENT_MOUSE_WHEEL, [this](SDL_Event &event) {
            glm::vec2 mouseScreenPos; // 屏幕空间坐标
            SDL_GetMouseState(&mouseScreenPos.x, &mouseScreenPos.y);
            glm::vec2 windowCenter = GET_RenderSystem.GetWindowCenter();

            glm::vec2 mouseOffset = mouseScreenPos - windowCenter;
            glm::vec2 worldPosBeforeZoom = mouseOffset * zoomR + position_ * 100.0f;
            constexpr float zoomFactor[2] =  {1.05f, 0.952381f};
            int index = (event.wheel.y > 0) ? 0 : 1; // 0: zoom in, 1: zoom out
            if (ZoomClamp(zoom, zoomFactor[index])) zoomR *= zoomFactor[(index + 1) & 1];
            position_ =( worldPosBeforeZoom - mouseOffset * zoomR) * 0.01f;
        }); 
}

