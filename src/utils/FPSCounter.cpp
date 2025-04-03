#include "FPSCounter.h"
#include <chrono>

void FPSCounter::StartFrame() {
    auto now = std::chrono::steady_clock::now();
    frameStartTime_ = now;

    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                     now - lastSecondTime_)
                     .count();

    if (delta >= 1000) {
        currentFPS_ = frameCount_;
        frameCount_ = 0;
        lastSecondTime_ = now;
    }

    frameCount_++;
}

void FPSCounter::EndFrame() {
    auto now = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>(
        now - frameStartTime_
    ).count();  

    lastFrameTime_ = (frameTime) * 0.001f;
}