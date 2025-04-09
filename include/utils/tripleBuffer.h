# pragma once

#include <cstddef>
#include <mutex>
#include <vector>

using FrameVersion = size_t;
template <typename T>
class TripleBuffer {
public:
    void AddCommand(T& item);
    void AddCommand(T&& item);
    void Submit();
    void SubmitBuffers(std::vector<T>& buffers);
    void Prepare();
    std::vector<T>& GetConsumeBuffer() noexcept;
    std::vector<T>& GetFrontBuffer() noexcept;
    void Reserve(size_t size);

private:
    std::mutex swap_mutex_;
    std::vector<T> front_;
    std::vector<T> middle_;
    std::vector<T> back_;

    FrameVersion backVersion_ = 0;
    FrameVersion midVersion_ = 1;

};

