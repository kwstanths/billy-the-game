#ifndef __MemoryPage_hpp__
#define __MemoryPage_hpp__

#include "debug_tools/Assert.hpp"

namespace memory_subsystem {

    /* Base type */
    typedef char BYTE;

    /**
        Holds a continuous chunk of memory. Should be allocated normally through the constructor
    */
    class MemoryPage {
    private:
        BYTE * mem_ = nullptr;
        size_t size_ = 0;

    public:
        /**
            Initializes a memory of bytes_size * BYTE, clear the memory to zero
            @param bytes_size The number of continuous bytes
        */
        MemoryPage(size_t bytes_size);

        /**
            Deallocates the memory
        */
        ~MemoryPage();

        /**
            Get a memory address from a byte index
            @param byte_index The first byte of the address
            @return The address
        */
        BYTE * Get(size_t byte_index);

        /**
            Get a memory address from a byte index, basic check for overflow
            @param byte_index The first byte of the address
            @param memory_size The size to check, and clear
            @return The address
        */
        BYTE * Get(size_t byte_index, size_t memory_size);

        /**
            Gets the size of the template type, calls Get with that size and casts the 
            memory address to the required type
            @param byte_index The first byte of the address
            @return The memory address
        */
        template<typename T> T * Get(size_t byte_index) {
            _assert(mem_ != nullptr);

            if (byte_index + sizeof(T) > size_) return nullptr;

            return reinterpret_cast<T *>(&mem_[byte_index]);
        }

    };

}


#endif