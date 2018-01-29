#include "PoolAllocator.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/Assert.hpp"
namespace dt = debug_tools;

namespace memory_subsystem {

    PoolAllocator::PoolAllocator() {
        is_inited_ = false;
    }

    PoolAllocator::~PoolAllocator() {
        Destroy();
    }

    bool PoolAllocator::Init(size_t block_size, size_t number_of_blocks) {
        if (is_inited_) return false;


        is_inited_ = true;
        return is_inited_;
    }

    bool PoolAllocator::Destroy() {
        if (!is_inited_) return false;

        is_inited_ = false;
        return true;
    }

    BYTE * PoolAllocator::Allocate() {
        return nullptr;
    }

    bool PoolAllocator::Deallocate() {
        return false;
    }

    size_t PoolAllocator::GetBytesAllocate() {
        return 0;
    }

    size_t PoolAllocator::GetBytesUsed() {
        return 0;
    }

}