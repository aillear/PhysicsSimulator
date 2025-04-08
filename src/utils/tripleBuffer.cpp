#include "tripleBuffer.h"
#include "renderSystem.h"
#include <cstddef>
#include <mutex>
#include <vector>

template <typename T>
void TripleBuffer<T>::AddCommand(T& item) {
    front_.emplace_back(item);
}

template <typename T>
void TripleBuffer<T>::AddCommand(T&& item) {
    front_.emplace_back(item);
}

template <typename T>
void TripleBuffer<T>::SubmitBuffers(std::vector<T>& buffers) {
    std::lock_guard<std::mutex> lock(swap_mutex_);
    middle_.swap(front_);
    front_.clear();
}

template <typename T>
void TripleBuffer<T>::Prepare() {
    std::lock_guard<std::mutex> lock(swap_mutex_);
    back_.swap(middle_);
}

template <typename T>
std::vector<T>& TripleBuffer<T>::GetConsumeBuffer() noexcept {
    return back_;
}

template <typename T>
void TripleBuffer<T>::FinishConsuming() noexcept {
    back_.clear();
}

template <typename T>
std::vector<T>& TripleBuffer<T>::GetFrontBuffer() noexcept {
    return front_;
}

template <typename T>
void TripleBuffer<T>::Reserve(size_t size) {
    front_.reserve(size);
    middle_.reserve(size);
    back_.reserve(size);
}

template class TripleBuffer<DrawCommand>;