#include "app.h"
#include <SDL.h>


int main(int argc, char *argv[]) {
    App::Instance().Init(argc, argv);
    App::Instance().Run();
    return 0;
}