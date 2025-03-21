#pragma once
#include <filesystem>

namespace fs = std::filesystem;

class PathMgr {
  public:
    static PathMgr& Instance();

    void Init();

    template <typename... Args>
    fs::path GetPath(Args&&... args) {
      return (path_ / ... / fs::path(std::forward<Args>(args)));
    }
  private:
    PathMgr() = default;
    PathMgr(const PathMgr&) = delete;
    PathMgr(const PathMgr&&) = delete;
    PathMgr& operator=(const PathMgr&) = delete;

    fs::path path_;
};

# define GET_PATH(...) \
    PathMgr::Instance().GetPath(__VA_ARGS__)
