# include "renderBufferMgr.h"
#include <cstddef>

RenderBufferMgr& RenderBufferMgr::Instance() {
    static RenderBufferMgr instance;
    return instance;
}

bool RenderBufferMgr::Init(size_t reserveSize) {
    buffer.Reserve(reserveSize);
    return true;
}

void RenderBufferMgr::Destroy(){;}