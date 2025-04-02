# include "pathMgr.h"
#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>

#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>

#elif defined(__APPLE__)
#include <mach-o/dyld.h>    
#endif

PathMgr& PathMgr::Instance() {
    static PathMgr instance;
    return instance;
}

// cross platform support
void PathMgr::Init(int rootCount) {
    std::string raw_path;

    #ifdef _WIN32
        std::vector<char> buffer(MAX_PATH);
        DWORD len;
        while ((len = GetModuleFileNameA(nullptr, buffer.data(), buffer.size())) == buffer.size()) {
            buffer.resize(buffer.size() * 2);
        }
        if (len > 0) raw_path.assign(buffer.data(), len);
    #elif defined(__linux__)
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, PATH_MAX);
        if (len != -1) raw_path.assign(buffer, len);
    #elif defined(__APPLE__)
        char buffer[PATH_MAX];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            raw_path.assign(buffer);
        }
    #endif
    
    if (raw_path.empty()) {
        throw std::runtime_error("Failed to get executable path");
    }

    path_ = fs::path(raw_path);
    while (rootCount > 0) {
        path_ = path_.parent_path();
        rootCount--;
    }
}
