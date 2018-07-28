#ifndef __MemoryManager_hpp__
#define __MemoryManager_hpp__

#include "PoolAllocator.hpp"
#include "ArrayAllocator.hpp"


namespace game_engine {
namespace memory {

    class MemoryManager {
    public:

        static MemoryManager & GetInstance() {
            static MemoryManager instance;
            return instance;
        }

        /**
            Be very carefull when destroying the asset manager, since it will delete all
            the memory allocated
        */
        ~MemoryManager();

        /**
            
        */
        ArrayAllocator * GetStaticObjectsAllocator();

        /**
            
        */
        PoolAllocator * GetRemovableObjectsAllocator();

        /**
            
        */
        PoolAllocator * GetPhysicsObjectsAllocator();

        /**
        
        */
        size_t GetMemoryAllocated();


    private:
        ArrayAllocator * static_objects_memory_allocator_;
        PoolAllocator * removable_objecs_memory_allocator_;
        PoolAllocator * physics_objects_memory_allocator_;

        /**
            Does nothing in particular. Call Init()
        */
        MemoryManager();
    };

}
}


#endif