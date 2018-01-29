#ifndef __PoolAllocator_hpp__
#define __PoolAllocator_hpp__

#include "MemoryPage.hpp"

namespace memory_subsystem {

    class PoolAllocator {
    public:
        PoolAllocator();

        ~PoolAllocator();

        bool Init(size_t block_size, size_t number_of_blocks);

        bool Destroy();

        BYTE * Allocate();

        bool Deallocate();

        size_t GetBytesAllocate();
        
        size_t GetBytesUsed();

    private:
        bool is_inited_;
        MemoryPage * page_;

    };

}

#endif