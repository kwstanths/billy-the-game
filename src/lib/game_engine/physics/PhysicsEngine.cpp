#include "PhysicsEngine.hpp"

#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace utl = game_engine::utility;


namespace game_engine {

namespace physics {
    
    PhysicsEngine::PhysicsEngine() {
        is_inited_ = false;
    }

    int PhysicsEngine::Init(Rectangle2D world_size, size_t number_of_objects) {

        if (!pool_quad_tree_.Init(sizeof(utility::QuadTree<PhysicsObject>), number_of_objects)) {
            dt::Console(dt::FATAL, "PhysicsEngine::Init(): PoolAllocato::Init() failed");
            return Error::ERROR_PHYSICS_INIT;
        }
        if (!world_.Init(Rectangle2D(world_size), &pool_quad_tree_)) {
            dt::Console(dt::FATAL, "Physicsengine::Init(): QuadTree::Init() failed");
            return Error::ERROR_PHYSICS_INIT;
        }

        is_inited_ = true;
        return 0;
    }

    int PhysicsEngine::Destroy() {

        pool_quad_tree_.Destroy();

        is_inited_ = false;
        return 0;
    }

    bool PhysicsEngine::IsInited() {
        return is_inited_;
    }

    int PhysicsEngine::Insert(PhysicsObject * object) {
        world_.Insert(Point2D(object->GetX(), object->GetY()), object);
        return 0;
    }

    int PhysicsEngine::Update(PhysicsObject * object, float new_pos_x, float new_pos_y) {
        world_.Update(Point2D(object->GetX(), object->GetY()), object, Point2D(new_pos_x, new_pos_y));
        return 0;
    }

    CollisionResult_t PhysicsEngine::CheckCollision(PhysicsObject * object, float move_offset, Direction direction) {
        CollisionResult_t collision_res;

        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): World sector is not initialised");
            return collision_res;
        }
        if (object == nullptr) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): moving objectis null");
            return collision_res;
        }
        if (!object->IsInited()) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): moving objectis not initialised");
            return collision_res;
        }
        if (direction < 0 || direction >= 360.0f) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): direction is out of the allowed values");
            return collision_res;
        }

        bool moving_top = ((direction >= 0.0f && direction < 90.0f) || (direction > 270.0f && direction <= 360.0f));
        bool moving_down = (direction > 90.0f && direction < 270.0f);
        bool moving_left = (direction > 0 && direction < 180.0f);
        bool moving_right = (direction > 180.0f && direction < 360.0f);

        Point2D moving_object_new_position(object->GetX(), object->GetY());
        if (moving_top) {
            moving_object_new_position.y_ += move_offset;
            collision_res.up_ = CollisionGetDistance(object, moving_object_new_position).first;
            moving_object_new_position.y_ = object->GetY() + collision_res.up_;
        } else if (moving_down) {
            moving_object_new_position.y_ -= move_offset;
            collision_res.down_ = CollisionGetDistance(object, moving_object_new_position).first;
            moving_object_new_position.y_ = object->GetY() - collision_res.up_;
        }

        if (moving_left) {
            moving_object_new_position.x_ -= move_offset;
            collision_res.left_ = CollisionGetDistance(object, moving_object_new_position).second;
            moving_object_new_position.x_ = object->GetX() - collision_res.left_;

        } else if (moving_right) {
            moving_object_new_position.x_ += move_offset;
            collision_res.right_ = CollisionGetDistance(object, moving_object_new_position).second;
            moving_object_new_position.x_ = object->GetX() - collision_res.right_;
        }

        return collision_res;
    }

    std::pair<float, float> PhysicsEngine::CollisionGetDistance(PhysicsObject * object, Point2D new_position) {
        /* Get neighbours */
        std::vector<PhysicsObject *> neighbours(50);
        size_t nof = world_.QueryRange(Rectangle2D(object->GetX(), object->GetY(), 3, 3), neighbours);
        
        float horizontal_move_offset = std::abs(object->GetX() - new_position.x_);
        float vertical_move_offset = std::abs(object->GetY() - new_position.y_);

        for (size_t i = 0; i < nof; i++) {
            PhysicsObject * neighbour = neighbours[i];

            if (object->Collides(new_position, neighbour)) {

                /* TODO apply some clever mechanism to calculate remaining distance to colliding object */
                return std::pair<float, float>(0.0f, 0.0f);
            }
        }

        return std::pair<float, float>(vertical_move_offset, horizontal_move_offset);
    }



}

}