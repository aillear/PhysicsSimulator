#include "vector2.h"
#include "logger.h"
#include <SDL.h>
#include <string>


int main(int argc, char* argv[]) {

    {
        Logger::Instance().Init(Logger::INFO, "app.log", true);
        Vector2<float> a(1.2, 2.3);
        Vector2f b(3.3, 2.2);
        
        Vector2d c(20231.23,3.3231434);
        Vector2i ddd(1,3);
        LOG_INFO("!231231");
        F_LOG_INFO("vector a is: {}, {:.4f}, {}", b, c, ddd);
        std::string str = "I want an ERROR, so there is an ERROR";
        F_LOG_ERROR("Error Occur:  {}", str);
    }

    return 0;
}