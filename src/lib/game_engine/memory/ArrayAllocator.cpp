#include "ArrayAllocator.hpp"

#include <cmath>

#include "debug_tools/Console.hpp"
#include "debug_tools/Assert.hpp"
namespace dt = debug_tools;


namespace game_engine{
namespace memory {
    
    ArrayAllocator::ArrayAllocator() {  
        is_inited_ = false;
    }

    ArrayAllocator::~ArrayAllocator() {
        Destroy();
    }

    bool ArrayAllocator::Init(size_t bytes_size) {
        if (is_inited_) return false;

        size_t number_of_pages = static_cast<size_t>(std::ceil(((double)bytes_size) / ((double)PAGE_SIZE)));
        pages_ = std::vector<MemoryPage *>(number_of_pages);
        for (size_t i = 0; i < number_of_pages; i++)
            pages_[i] = new MemoryPage(PAGE_SIZE);

        page_offset_ = 0;
        current_page_ = 0;

        is_inited_ = true;
        return is_inited_;
    }

    bool ArrayAllocator::Destroy() {
        for (size_t i = 0; i < pages_.size(); i++) {
            delete pages_[i];
            pages_[i] = nullptr;
        }
        pages_.clear();

        page_offset_ = 0;
        current_page_ = 0;

        is_inited_ = false;
        return true;
    }

    BYTE * ArrayAllocator::Allocate(size_t bytes) {
        if (!is_inited_) return nullptr;

        BYTE * address = pages_[current_page_]->Get(page_offset_, bytes);
        if (address == nullptr) {
            current_page_++;
            page_offset_ = 0;
            if (current_page_ == pages_.size()) {
                pages_.push_back(new MemoryPage(PAGE_SIZE));
            }

            address = pages_[current_page_]->Get(page_offset_, bytes);
            /* If address is nullptr here, then the MEMORY_PAGE used is not big enough to hold the bytes */
        } 
        page_offset_ += bytes;
       
        return address;
    }

    BYTE * ArrayAllocator::AllocateAligned(size_t bytes, size_t alignment) {
        if (!is_inited_) return nullptr;
        _assert(alignment == 2 || alignment == 4 || alignment == 16);

        size_t expanded_size = bytes + alignment;
        BYTE * unaligned_address = Allocate(expanded_size);

        size_t mask = (alignment - 1);
        size_t misalignment = unaligned_address && mask;
        size_t adjustment = alignment - misalignment;

        BYTE * aligned_address = unaligned_address + adjustment;
        return aligned_address;
    }

    size_t ArrayAllocator::GetBytesAllocated() {
        return pages_.size() * PAGE_SIZE;
    }

    size_t ArrayAllocator::GetBytesUsed() {
        return current_page_ * PAGE_SIZE + page_offset_;
    }



}

}

