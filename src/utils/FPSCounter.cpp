#include "FPSCounter.h"

void FPSCounter::StartFrame() {
    auto now = std::chrono::steady_clock::now();
    frameStartTime_ = now;

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
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
    auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - frameStartTime_
    ).count();  

    lastFrameTime_ = static_cast<int>(frameTime);
}