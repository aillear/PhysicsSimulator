#pragma once

#include "objectWorld.h"
#include <glm/ext/vector_float2.hpp>

class Camera : public ObjectWorld {
  public:
    Camera(glm::vec2 position, float zoom = 1) : ObjectWorld(position), zoom(zoom), zoomR(1/zoom) {;}
    ~Camera() override = default;
    // useless methods.
    // camera is directly managed by RenderSystem.
    // no one w
    void Init() override;
    void Render() override {;}
    void Update(float dt) override {;}
    void PhysicsUpdate(float dt) override {;}
    void HandleEvent(SDL_Event &event) override {;}
    void OnMouseMove(SDL_Event &event) override {;}
    void OnMouseDown(SDL_Event &event) override {;}
    void OnMouseUp(SDL_Event &event) override {;}
    void OnKeyDown(SDL_Event &event) override {;}
    void OnKeyUp(SDL_Event &event) override {;}
    void Destroy() override {;}

    // useful methods.
    float getZoom() const { return zoom; }
    void setZoom(float zoom) { this->zoom = zoom; this->zoomR = 1 / zoom; }
    float getZoomR() const { return zoomR; }
    void setZoomR(float zoomR) { this->zoomR = zoomR; this->zoom = 1 / zoomR; }


  private:
    bool isDragging = false;
    float zoom;
    float zoomR;
    glm::vec2 lastPosition;

};