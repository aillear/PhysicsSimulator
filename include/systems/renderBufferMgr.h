# pragma once

#include "tripleBuffer.h"
#include "renderSystem.h"
#include <cstddef>
#include <utility>
#include <vector>
class RenderBufferMgr {
    public:
    static RenderBufferMgr& Instance();
    bool Init(size_t reserveSize = 1024);
    void Destroy();

    void AddCommand(DrawCommand& item) {buffer.AddCommand(std::forward<DrawCommand>(item));}
    void AddCommand(DrawCommand&& item) {buffer.AddCommand(std::forward<DrawCommand>(item));}
    void Submit() {buffer.Submit();}
    void SubmitBuffers(std::vector<DrawCommand>& buffers) {buffer.SubmitBuffers(buffers);}
    void Prepare() {buffer.Prepare();}
    std::vector<DrawCommand>& GetConsumeBuffer() noexcept {return buffer.GetConsumeBuffer();}
    std::vector<DrawCommand>& GetFrontBuffer() noexcept {return buffer.GetFrontBuffer();}
    void Reverse(size_t size) {buffer.Reserve(size);}
    TripleBuffer<DrawCommand> buffer;
};

# define GET_Buffer RenderBufferMgr::Instance()