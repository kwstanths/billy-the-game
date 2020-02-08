#include "MemoryManager.hpp"

#include <exception>

#include "game_engine/utility/QuadTree.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/graphics/Light.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/ErrorCodes.hpp"

namespace dt = debug_tools;


namespace game_engine {
namespace memory {

    size_t STATIC_OBJETCS_MEMORY_SIZE = 500 * 500;

    size_t REMOVABLE_OBJECTS_MEMORY_BLOCK_SIZE = 400;
    size_t REMOVABLE_OBJECTS_MEMORY_BLOCKS_NUMBER = 1000;

    size_t LIGHT_OBJECTS_MEMORY_BLOCKS_SIZE = sizeof(utility::QuadTree<graphics::PointLight *>);
    size_t LIGHT_OBJECTS_MEMORY_BLOCKS_NUMBER = 10000;

    MemoryManager::MemoryManager() {

        static_objects_memory_allocator_ = new ArrayAllocator();
        static_objects_memory_allocator_->Init(STATIC_OBJETCS_MEMORY_SIZE);
        
        removable_objecs_memory_allocator_ = new PoolAllocator();
        removable_objecs_memory_allocator_->Init(REMOVABLE_OBJECTS_MEMORY_BLOCK_SIZE, REMOVABLE_OBJECTS_MEMORY_BLOCKS_NUMBER);

        world_lights_memory_allocator_ = new PoolAllocator();
        world_lights_memory_allocator_->Init(LIGHT_OBJECTS_MEMORY_BLOCKS_SIZE, LIGHT_OBJECTS_MEMORY_BLOCKS_NUMBER);
    }

    MemoryManager::~MemoryManager() {
        static_objects_memory_allocator_->Destroy();
        removable_objecs_memory_allocator_->Destroy();
        world_lights_memory_allocator_->Destroy();
    }

    ArrayAllocator * MemoryManager::GetStaticObjectsAllocator() {
        return static_objects_memory_allocator_;
    }

    PoolAllocator * MemoryManager::GetRemovableObjectsAllocator() {
        return removable_objecs_memory_allocator_;
    }

    PoolAllocator * MemoryManager::GetWorldLightsAllocator() {
        return world_lights_memory_allocator_;
    }

    size_t MemoryManager::GetMemoryAllocated() {
        return static_objects_memory_allocator_->GetBytesAllocated() +
            removable_objecs_memory_allocator_->GetBytesAllocated();
    }


}
}
