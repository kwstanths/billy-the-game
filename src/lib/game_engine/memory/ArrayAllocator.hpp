#ifndef __ArrayAllocator_hpp__
#define __ArrayAllocator_hpp__

#include <vector>

#include "MemoryPage.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


namespace game_engine {

namespace memory_subsystem {

    /* Page size to be used in bytes */
    static const size_t PAGE_SIZE = 8192;

    /**
        Allocate memory in a sequential manner for differently sized objects
        IS NOT thread safe
    */
    class ArrayAllocator {
    public:
        /**
            Does nothing in particular
        */
        ArrayAllocator();

        /**
            Calls Destroy()
        */
        ~ArrayAllocator();

        /* Delete all other constuctors available */
        ArrayAllocator(const ArrayAllocator& other) = delete;
        ArrayAllocator(const ArrayAllocator&& other) = delete;
        ArrayAllocator& operator=(const ArrayAllocator& other) = delete;
        ArrayAllocator& operator=(ArrayAllocator&& other) = delete;

        /**
            Initializes an array of pointers to MemoryPage objects of size PAGE_SIZE,
            so as to fit the bytes_size. Allocations will sequentially get memory from
            the currently allocated pages
            @param bytes_size The bytes to allocate
            @return true = OK, false = NOT OK
        */
        bool Init(size_t bytes_size);

        /**
            Deallocates all allocated MemoryPage objects, needs to call Init() afterwards
            @return true = OK, false = NOT OK
        */
        bool Destroy();

        /**
            Get memory for {bytes} size, if {bytes} does not fit inside the last page,
            a new memory page will be allocated. If {bytes} size is bigger than the size 
            of a single MemoryPage object, the function will return nullptr
            @param bytes The size to allocate
            @return The memory address, if nullptr then bytes does not fit inside a MemoryPage
        */
        BYTE * Allocate(size_t bytes);

        /**
            Gets the size of the template type, calls Allocate with that size, and casts the
            memory address to the required type
            @return The memory address
        */
        template<typename T> T * Allocate() {
            if (!is_inited_) return nullptr;

            size_t bytes = sizeof(T);
            BYTE * byte_address = Allocate(bytes);
            
            T * address = reinterpret_cast<T *>(byte_address);

            return address;
        }

        /**
            Get memory for {bytes} size with proper alignement, with the same logic applied to 
            Allocate() function
            @param bytes The size to allocate
            @return alignment The alignment should be 2 or 4 or 16
        */
        BYTE * AllocateAligned(size_t bytes, size_t alignment);

        /**
            Gets the size of the template typem calls AllocateAligned with that size and alignment,
            and casts the result to the required type
            @return The memory address
        */
        template<typename T> T * AllocateAligned(size_t alignment) {
            if (!is_inited_) return nullptr;

            size_t bytes = sizeof(T);
            BYTE * byte_address = AllocateAligned(bytes, alignment);

            T * address = reinterpret_cast<T *>(byte_address);

            return address;
        }

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
        bool is_inited_ = false;
        std::vector<MemoryPage *> pages_;
        size_t page_offset_;
        size_t current_page_;

    };

}

}


#endif
