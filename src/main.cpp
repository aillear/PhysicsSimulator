#include "initialization.h"
#include <SDL.h>
#include <string>
#include "logger.h"



int main(int argc, char* argv[]) {

    {
        Initialization();
        std::string str = "I want an ERROR, so there is an ERROR";
        F_LOG_ERROR("Error Occur:  {}", str);
    }

    
    return 0;
}