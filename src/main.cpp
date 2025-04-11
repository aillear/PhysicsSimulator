#include "app.h"
#include "physicsSystem.h"
#include "test.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {
    App::Instance().Init(argc, argv);
    App::Instance().AddFuntionBeforeRun(SomeCustomLogicHere);
    PhysicsSystem::Instance().AddCustomInitFunction(SomeCustomLogicPHere);
    PhysicsSystem::Instance().AddCustomAfterUpdateFunction(SomeCustomLogicPAHere);
    App::Instance().Run();
    App::Instance().Destroy();
    return 0;
}