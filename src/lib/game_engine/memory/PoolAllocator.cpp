#include "PoolAllocator.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/Assert.hpp"
namespace dt = debug_tools;


namespace game_engine {
namespace memory {

    PoolAllocator::PoolAllocator() {
        is_inited_ = false;
    }

    PoolAllocator::~PoolAllocator() {
        Destroy();
    }

    bool PoolAllocator::Init(size_t block_size_bytes, size_t number_of_blocks) {
        if (is_inited_) return false;

        if (block_size_bytes < sizeof(void*)) {
            dt::ConsoleInfoL(dt::CRITICAL, "PoolAllocator::Init() block size is smaller than a pointer",
                "block_size", block_size_bytes,
                "pointer size", sizeof(void*));
            return false;
        }
        
        block_size_bytes_ = block_size_bytes;
        number_of_blocks_ = number_of_blocks;

        page_ = new MemoryPage(number_of_blocks * block_size_bytes);

        /* 
            Initialize freelist. Store pointers inside the memory itself
            nullptr for the last is initialised from MemoryPage constructor due to memset to zero
        */
        for (size_t i = 0; i < number_of_blocks - 1; i++){
            node_t * current = page_->Get<node_t>(block_size_bytes * i);
            current->next_ = page_->Get<node_t>(block_size_bytes * (i + 1));
        }
        freelist_ = page_->Get<node_t>(0);
        free_blocks_ = number_of_blocks;

        is_inited_ = true;
        return is_inited_;
    }

    bool PoolAllocator::Destroy() {
        if (!is_inited_) return false;

        delete page_;
        page_ = nullptr;
        freelist_ = nullptr;
        free_blocks_ = 0;

        is_inited_ = false;
        return true;
    }

    BYTE * PoolAllocator::Allocate() {
        if (!is_inited_) return nullptr;

        if (freelist_ == nullptr) {
            dt::Console(dt::FATAL, "PoolAllocator::Allocate() Out of free nodes");
            return nullptr;
        }
        
        /* Get the first memory block */
        node_t * memory_block = freelist_;

        /* Update free list head */
        freelist_ = freelist_->next_;
        free_blocks_--;

        /* Initialize memory */
        BYTE * address = reinterpret_cast<BYTE *>(memory_block);
        memset(address, 0, block_size_bytes_);

        return address;
    }

    bool PoolAllocator::Deallocate(void * address) {
        if (!is_inited_) return false;

        if (address == nullptr) return false;

        memset(address, 0, block_size_bytes_);
        node_t * new_freelist_node = reinterpret_cast<node_t *>(address);
        new_freelist_node->next_ = freelist_;
        freelist_ = new_freelist_node;
        free_blocks_++;

        return true;
    }

    bool PoolAllocator::IsInited(){
        return is_inited_;
    }

    size_t PoolAllocator::GetBlockSize(){
        return block_size_bytes_;
    }

    size_t PoolAllocator::GetBytesAllocated() {
        return block_size_bytes_ * number_of_blocks_;
    }

    size_t PoolAllocator::GetBytesUsed() {
        return (number_of_blocks_ - free_blocks_) * block_size_bytes_;
    }

}

}