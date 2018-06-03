#ifndef __PhysicsEngine_hpp__
#define __PhysicsEngine_hpp__

#include "PhysicsObject.hpp"

#include "game_engine/memory/PoolAllocator.hpp"

#include "game_engine/utility/QuadTree.hpp"

#include "game_engine/Collision.hpp"

namespace game_engine {

namespace physics {

    class PhysicsEngine {
    public:
        PhysicsEngine();
    
        int Init(Rectangle2D world_size, size_t number_of_objects);

        int Destroy();
    
        bool IsInited();

        int Insert(PhysicsObject * object);

        int Update(PhysicsObject * object, float new_pos_x, float new_pos_y);

        CollisionResult_t CheckCollision(PhysicsObject * object, float move_offset, Direction direction);
    
    private:
        bool is_inited_;
        memory_subsystem::PoolAllocator pool_quad_tree_;
        utility::QuadTree<PhysicsObject> world_;

        std::pair<float, float> CollisionGetDistance(PhysicsObject * object, Point2D new_position);
    };

}

}


#endif