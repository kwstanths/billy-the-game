#include "MemoryPage.hpp"

#include <string>


namespace memory_subsystem {

    MemoryPage::MemoryPage(size_t bytes_size) {
        size_ = bytes_size;
        mem_ = new BYTE[bytes_size];
        memset(&mem_[0], 0, bytes_size);
    }

    MemoryPage::~MemoryPage() {
        delete mem_;
    }

    BYTE * MemoryPage::Get(size_t byte_index) {
        _assert(mem_ != nullptr);
        
        if (byte_index > size_) return nullptr;

        return &mem_[byte_index];
    }

    BYTE * MemoryPage::Get(size_t byte_index, size_t memory_size) {
        _assert(mem_ != nullptr);

        if (byte_index + memory_size > size_) return nullptr;

        return &mem_[byte_index];
    }

}