#include "app.h"
#include "physicsSystem.h"
#include "test.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {
    App::Instance().Init(argc, argv);
    App::Instance().AddCustomInit(InitApp);
    App::Instance().AddCustomUpdate(UpdateApp);
    PhysicsSystem::Instance().AddCustomInitFunction(InitPhy);
    PhysicsSystem::Instance().AddCustomAfterUpdateFunction(UpdatePhy);
    App::Instance().Run();
    App::Instance().Destroy();
    return 0;
}