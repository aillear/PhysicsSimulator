#include "app.h"
#include <SDL.h>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int GRID_SIZE = 50;
const int MIN_GRID_SIZE = 10;
const int MAX_GRID_SIZE = 200;

int main(int argc, char *argv[]) {
    App::Instance().Init(argc, argv);
    App::Instance().Run();
    return 0;
}