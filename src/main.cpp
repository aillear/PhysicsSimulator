#include "app.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {
    App::Instance().Init(argc, argv);
    App::Instance().Run();
    return 0;
}