#include "initialization.h"
#include "pathMgr.h"
#include "logger.h"


void Initialization() {
    PathMgr::Instance().Init();
    Logger::Instance().Init(Logger::INFO, "app.log", true);
    LOG_INFO("there is a info");
}