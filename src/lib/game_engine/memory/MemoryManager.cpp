#include "MemoryManager.hpp"

#include <exception>

#include "game_engine/utility/QuadTree.hpp"
#include "game_engine/physics/PhysicsObject.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"

#include "debug_tools/Console.hpp"

#include "game_engine/ErrorCodes.hpp"

namespace dt = debug_tools;


namespace game_engine {
namespace memory {

    MemoryManager::MemoryManager() {

        static_objects_memory_allocator_ = new ArrayAllocator();
        static_objects_memory_allocator_->Init(500 * 500);
        
        removable_objecs_memory_allocator_ = new PoolAllocator();
        removable_objecs_memory_allocator_->Init(400, 1000);
        
        physics_objects_memory_allocator_ = new PoolAllocator();
        physics_objects_memory_allocator_->Init(sizeof(utility::QuadTree<physics::PhysicsObject *>), 200000);

        world_lights_memory_allocator_ = new PoolAllocator();
        world_lights_memory_allocator_->Init(sizeof(utility::QuadTree<graphics::PointLight_t *>), 100000);
    }

    MemoryManager::~MemoryManager() {
        static_objects_memory_allocator_->Destroy();
        removable_objecs_memory_allocator_->Destroy();
    }

    ArrayAllocator * MemoryManager::GetStaticObjectsAllocator() {
        return static_objects_memory_allocator_;
    }

    PoolAllocator * MemoryManager::GetRemovableObjectsAllocator() {
        return removable_objecs_memory_allocator_;
    }

    PoolAllocator * MemoryManager::GetPhysicsObjectsAllocator() {
        return physics_objects_memory_allocator_;
    }

    PoolAllocator * MemoryManager::GetWorldLightsAllocator() {
        return world_lights_memory_allocator_;
    }

    size_t MemoryManager::GetMemoryAllocated() {
        return static_objects_memory_allocator_->GetBytesAllocated() +
            removable_objecs_memory_allocator_->GetBytesAllocated() +
            physics_objects_memory_allocator_->GetBytesAllocated();
    }


}
}
