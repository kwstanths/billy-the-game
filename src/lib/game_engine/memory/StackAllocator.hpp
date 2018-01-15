#ifndef __StackAllocator_hpp__
#define __StackAllocator_hpp__


#include "MemoryPage.hpp"


namespace memory_subsystem {

    class StackAllocator {
    private:
        bool is_inited_ = false;
        MemoryPage * page_ = nullptr;
        size_t index_ = 0;

    public:
        StackAllocator();

        void Init(size_t stack_size);

        void Destroy();

    };

}


#endif