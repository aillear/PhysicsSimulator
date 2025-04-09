#include "tripleBuffer.h"
#include "renderSystem.h"
#include <cstddef>
#include <utility>
#include <vector>
#include <mutex>

template <typename T>
void TripleBuffer<T>::AddCommand(T& item) {
    front_.emplace_back(item);
}

template <typename T>
void TripleBuffer<T>::AddCommand(T&& item) {
    front_.emplace_back(std::move(item));
}

template <typename T>
void TripleBuffer<T>::Submit() {
    std::lock_guard<std::mutex> lock(swap_mutex_);
    middle_.swap(front_);
    midVersion_++;
    front_.clear();
}

template <typename T>
void TripleBuffer<T>::SubmitBuffers(std::vector<T>& buffers) {
    back_ = buffers;
    Submit();
}

template <typename T>
void TripleBuffer<T>::Prepare() {
    std::lock_guard<std::mutex> lock(swap_mutex_);
    if (backVersion_ >= midVersion_) return;
    
    back_.swap(middle_);
    backVersion_ = midVersion_;
}

template <typename T>
std::vector<T>& TripleBuffer<T>::GetConsumeBuffer() noexcept {
    return back_;
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