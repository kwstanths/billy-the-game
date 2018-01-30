#ifndef __PoolAllocator_hpp__
#define __PoolAllocator_hpp__

#include "MemoryPage.hpp"

namespace memory_subsystem {

    class PoolAllocator {
    public:
        PoolAllocator();

        ~PoolAllocator();

        PoolAllocator(const PoolAllocator& other) = delete;
        PoolAllocator(const PoolAllocator&& other) = delete;
        PoolAllocator& operator=(const PoolAllocator& other) = delete;
        PoolAllocator& operator=(PoolAllocator&& other) = delete;

        bool Init(size_t block_size_bytes, size_t number_of_blocks);

        bool Destroy();

        BYTE * Allocate();

        template<typename T> T * Allocate() {
            if (!is_inited_) return nullptr;

            BYTE * address = Allocate();

            return reinterpret_cast<T *>(address);
        }

        bool Deallocate(void * address);

        size_t GetBytesAllocated();
        
        size_t GetBytesUsed();

    private:
        typedef struct node {
            struct node * next_;
        } node_t ;

        bool is_inited_;
        size_t block_size_bytes_;
        size_t number_of_blocks_;

        MemoryPage * page_;
        node_t * freelist_;

    };

}

#endif