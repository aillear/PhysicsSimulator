#include "configs.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_stdinc.h"


Uint32 USER_EVENT_SLOW_UPDATE;

Configer &Configer::Instance() {
    static Configer instance;
    return instance;
}

void Configer::Init() {
    USER_EVENT_SLOW_UPDATE = SDL_RegisterEvents(1);
}
void Configer::Destroy() { ; }