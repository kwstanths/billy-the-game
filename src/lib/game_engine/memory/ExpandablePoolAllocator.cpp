#include "ExpandablePoolAllocator.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/Assert.hpp"
namespace dt = debug_tools;


namespace game_engine {
namespace memory {

    ExpandablePoolAllocator::ExpandablePoolAllocator() {
        is_inited_ = false;
        memory_blocks_ = std::vector<PoolAllocator *>(1, nullptr);

        for (size_t i = 0; i < memory_blocks_.size(); i++) {
            memory_blocks_[i] = new PoolAllocator();
        }
    }

    ExpandablePoolAllocator::~ExpandablePoolAllocator() {
        Destroy();
    }

    bool ExpandablePoolAllocator::Init(size_t block_size_bytes) {
        
        if (is_inited_) return false;

        for (size_t i = 0; i < memory_blocks_.size(); i++) {
            bool ret = memory_blocks_[i]->Init(block_size_bytes, BLOCKS_PER_POOL);
            if (!ret) return false;
        }

        block_size_bytes_ = block_size_bytes;
        is_inited_ = true;
        return true;
    }

    bool ExpandablePoolAllocator::Init(size_t block_size_bytes, size_t blocks_per_pool) {
        
        BLOCKS_PER_POOL = blocks_per_pool;

        return ExpandablePoolAllocator::Init(block_size_bytes);
    }

    bool ExpandablePoolAllocator::Destroy() {

        if (!is_inited_) return false;
        
        for (size_t i = 0; i < memory_blocks_.size(); i++) {
            memory_blocks_[i]->Destroy();
            delete memory_blocks_[i];
        }

        is_inited_ = false;
        return true;
    }

    BYTE * ExpandablePoolAllocator::Allocate() {
        
        if (!is_inited_) return nullptr;

        BYTE * address = nullptr;
        for (size_t i = 0; i < memory_blocks_.size(); i++) {
            address = memory_blocks_[i]->Allocate();
            if (address != nullptr) {
                return address;
            }
        }

        PoolAllocator * pool_allocator = new PoolAllocator();
        memory_blocks_.push_back(pool_allocator);
        
        pool_allocator->Init(block_size_bytes_, BLOCKS_PER_POOL);
        address = pool_allocator->Allocate();
        return address;
    }

    bool ExpandablePoolAllocator::Deallocate(void * address) {

        if (!is_inited_) return false;

        _assert(address != nullptr);

        for (size_t i = 0; i < memory_blocks_.size(); i++) {
            PoolAllocator * pool_allocator = memory_blocks_[i];
            if (pool_allocator->IsAddressInside(address)) {
                pool_allocator->Deallocate(address);
                return true;
            }
        }

        dt::Console(dt::CRITICAL, "PoolAllocator::Deallocate(): Adress not in range");
        return false;
    }

    bool ExpandablePoolAllocator::IsInited() {
        return is_inited_;
    }

    size_t ExpandablePoolAllocator::GetBlockSize() {
        return block_size_bytes_;
    }

    size_t ExpandablePoolAllocator::GetBytesAllocated() {
        size_t total_bytes_allocated_ = 0;
        
        for (size_t i = 0; i < memory_blocks_.size() - 1; i++)
            total_bytes_allocated_ += memory_blocks_[i]->GetBytesAllocated();

        return total_bytes_allocated_;
    }

    size_t ExpandablePoolAllocator::GetBytesUsed() {
        size_t total_bytes_used = 0;

        for (size_t i = 0; i < memory_blocks_.size() - 1; i++)
            total_bytes_used += memory_blocks_[i]->GetBytesUsed();

        return total_bytes_used;
    }

}
}