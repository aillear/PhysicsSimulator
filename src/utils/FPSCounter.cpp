#include "FPSCounter.h"
#include <chrono>
#include <ratio>

void FPSCounter::StartFrame() {
    auto now = std::chrono::steady_clock::now();
    frameStartTime_ = now;

    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                     now - lastSecondTime_)
                     .count();

    frameCount_++;
    if (delta >= 1'000'000) {
        currentFPS_ = frameCount_;
        frameCount_ = 0;
        lastSecondTime_ = now;
    }
}

void FPSCounter::EndFrame() {
    auto now = std::chrono::steady_clock::now();
    auto frameTime =
        std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
            now - frameStartTime_);

    lastFrameTime_ = frameTime.count();
    lastFrameSecond_ = lastFrameTime_ * 0.001f;
}