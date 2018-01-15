#include "StackAllocator.hpp"

namespace memory_subsystem {

    StackAllocator::StackAllocator() {

    }

    void StackAllocator::Init(size_t stack_size) {
        page_ = new MemoryPage(stack_size);
        index_ = 0;
        is_inited_ = true;
    }

    void StackAllocator::Destroy() {
        delete page_;
        index_ = 0;
        is_inited_ = false;
    }

}