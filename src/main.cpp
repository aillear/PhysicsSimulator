#include "vector2.h"
#include <SDL.h>
#include <format>
#include "logger.h"

int main(int argc, char* argv[]) {

    {
        Logger::Instance().Init(Logger::INFO, "app.log", true);
        Vector2<float> a(1.2, 2.3);
        Vector2f b(3.3, 2.2);
        
        Vector2d c(20231.23,3.3231434);
        Vector2i ddd(1,3);
        LOG_INFO(std::format("vector a is: {}, {:.4f}, {}", b, c, ddd));
    }

    return 0;
}