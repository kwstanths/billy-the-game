#ifndef __PoolAllocator_hpp__
#define __PoolAllocator_hpp__

#include "MemoryPage.hpp"

namespace memory_subsystem {

    /**
        Allocate in memory in blocks stored sequentially on the heap
        IS NOT thread safe
    */
    class PoolAllocator {
    public:
        /**
            Does nothing in particular
        */
        PoolAllocator();

        /**
            Calls Destory()
        */
        ~PoolAllocator();

        /* Delete all other constuctors available */
        PoolAllocator(const PoolAllocator& other) = delete;
        PoolAllocator(const PoolAllocator&& other) = delete;
        PoolAllocator& operator=(const PoolAllocator& other) = delete;
        PoolAllocator& operator=(PoolAllocator&& other) = delete;

        /**
            Initializes a single MemoryPage with size so as to fit number_of_blocks * blocks_size_bytes
            @param blocks_size_bytes The size of a single block of memory in bytes
            @param number_of_blocks The total number of blocks
            @return true = OK, false = not OK
        */
        bool Init(size_t block_size_bytes, size_t number_of_blocks);

        /**
            Destroys the memory page, needs to call Init() afterwards
            @return true = OK, false = NOT OK
        */
        bool Destroy();

        /**
            Get an available block of memory, if none is available nullptr is returned
            @return The address of the first byte of the block, null if not initialised or out of memory
        */
        BYTE * Allocate();

        /**
            Get an available block of memory, if none is available nullptr is returned, the result
            is casted to the specified type, if the type does not fit inside the block of memory, 
            nullptr is returned
            @return The address of the first byte of the block, null if not initialised or out of memory
                or the type does not fit inside the block
        */
        template<typename T> T * Allocate() {
            if (!is_inited_) return nullptr;

            if (sizeof(T) > block_size_bytes_) return nullptr;

            BYTE * address = Allocate();

            return reinterpret_cast<T *>(address);
        }

        /**
            Frees the block of memory that starts from the address, the block will be inserted in a list
            with all the blocks that are available to be allocated again. PoolAllocator does not keep 
            track of the addresses that blocks start within the memory, neither keeps track if 
            Deallocate() is called with an address-block that is not allocated. That means that all hell 
            can break loose if called with an address that is not returned from the Allocate() function 
            or if called with a block-address that is already free. ALWAYS nullify your pointers after a call to 
            this function
            @param address The first byte of the block, as returned from Allocate()
            @return true = OK, false = NOT OK
        */
        bool Deallocate(void * address);

        /**
            Check if the object is initialised
            @return true = initialised, false = not initialised
        */
        bool IsInited();

        /**
            Get the number of bytes allocated, not the ones used, but the whole memory allocated
            @return Memory allocated
        */
        size_t GetBytesAllocated();
        
        /**
            Get the number of bytes used
        */
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
        size_t free_blocks_;

    };

}

#endif