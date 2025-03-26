# pragma once

#include <memory>
#include <mutex>
#include <vector>


template <typename T>
class DoubleBufferQueue {
public:

    void Write(const T& data);

    void Swap();

    std::shared_ptr<const std::vector<T>> GetData();

private:
    std::shared_ptr<std::vector<T>> backBuffer;
    std::shared_ptr<const std::vector<T>> frontBuffer;
    std::mutex writeMutex;
    std::mutex swapMutex;
};