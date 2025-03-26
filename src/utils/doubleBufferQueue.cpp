#include "doubleBufferQueue.h"
#include <memory>
#include <mutex>
#include <vector>


template <typename T>
void DoubleBufferQueue<T>::Write(const T& data) {
    std::lock_guard<std::mutex> lock(writeMutex);
    if (!backBuffer) {
        backBuffer = std::make_shared<std::vector<T>>();
    }
    backBuffer->push_back(data);
}

template<typename T>
void DoubleBufferQueue<T>::Swap() {
    std::lock_guard<std::mutex> lock(swapMutex);
    frontBuffer = std::atomic_exchange(&backBuffer, std::make_shared<std::vector<T>>());
}

template<typename T>
std::shared_ptr<const std::vector<T>> DoubleBufferQueue<T>::GetData() {
    return std::atomic_load(&frontBuffer);
}