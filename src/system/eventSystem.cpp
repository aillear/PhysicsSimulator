#include "eventSystem.h"
#include "renderSystem.h"
#include "app.h"
#include <SDL_events.h>


EventSystem& EventSystem::Instance() {
    static EventSystem instance;
    return instance;
}

void EventSystem::HandleEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            GET_App.running = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                GET_RenderSystem.SetWindowSize({event.window.data1, event.window.data2});
            break;
        }
        
    }
}