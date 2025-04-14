# pragma once   

#include <chrono>
class FPSCounter {
public:
    void StartFrame();
    void EndFrame();
    int GetFPS() const { return currentFPS_; }
    float GetLastFrameTime() const { return lastFrameTime_; }
    float GetLastFrameSecond() const { return lastFrameSecond_; }

private:
    
    std::chrono::steady_clock::time_point lastSecondTime_ = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point frameStartTime_;
    int frameCount_ = 0;
    int currentFPS_ = 0;
    float lastFrameTime_ = 0;
    float lastFrameSecond_ = 0;
};
