#ifndef __ExpandablePoolAllocator_hpp__
#define __ExpandablePoolAllocator_hpp__

#include "MemoryPage.hpp"
#include "PoolAllocator.hpp"

namespace game_engine {
namespace memory {

    /**
        Allocate memory in blocks stored sequentially on the heap
        IS NOT thread safe
    */
    class ExpandablePoolAllocator {
    private:

        size_t BLOCKS_PER_POOL = 2048;

    public:
        ExpandablePoolAllocator();

        ~ExpandablePoolAllocator();

        /**
            Initialize the pool allocator for a certain block size
            @param block_size_bytes The size of a block
            @return true = OK, false = NOT OK
        */
        bool Init(size_t block_size_bytes);

        /**
            Initialize the pool allocator for a certain block size, and a certain number of blocks per pool
            @param block_size_bytes The size of a block
            @param blocks_per_pool The number of blocks per pool
            @return true = OK, false = NOT OK
        */
        bool Init(size_t block_size_bytes, size_t blocks_per_pool);

        /**
               Destroys the pool allocator, needs to call Init() afterwards
               @return true = OK, false = NOT OK
           */
        bool Destroy();

        /**
            Get an available block of memory
            @return The address of the first byte of the block or nullptr if not initialised 
        */
        BYTE * Allocate();

        /**
            Get an available block of memory that is casted to the specified type, if the type does not fit inside the 
            block of memory, nullptr is returned
            @return The address of the first byte of the block, null if not initialised or the type does not fit inside the block
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
            Get the size of the block
            @return The block size
        */
        size_t GetBlockSize();

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
        std::vector<PoolAllocator *> memory_blocks_;
        bool is_inited_;
        size_t block_size_bytes_;
    };

}
}

#endif