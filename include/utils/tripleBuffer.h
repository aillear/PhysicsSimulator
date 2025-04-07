# pragma once

#include <mutex>
#include <vector>


template <typename T>
class TripleBuffer {
public:
    void AddCommand(T& item);
    void AddCommand(T&& item);
    void SubmitBuffers(std::vector<T>& buffers);
    void Prepare();
    std::vector<T>& GetComsumeBuffer() noexcept;
    void FinishConsuming() noexcept;
    std::vector<T>& GetFrontBuffer() noexcept;

private:
    std::mutex swap_mutex_;
    std::vector<T> front_;
    std::vector<T> middle_;
    std::vector<T> back_;
};