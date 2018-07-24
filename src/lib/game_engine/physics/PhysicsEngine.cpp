#include "PhysicsEngine.hpp"

#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace utl = game_engine::utility;
namespace math = game_engine::math;
namespace ge = game_engine;

namespace game_engine {
namespace physics {
    
    PhysicsEngine::PhysicsEngine() {
        is_inited_ = false;
    }

    PhysicsEngine::~PhysicsEngine() {
        Destroy();
    }

    int PhysicsEngine::Init(math::Rectangle2D world_size, size_t number_of_objects) {

        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        if (!pool_quad_tree_.Init(sizeof(utility::QuadTree<PhysicsObject *>), number_of_objects)) {
            dt::Console(dt::FATAL, "PhysicsEngine::Init(): PoolAllocato::Init() failed");
            return Error::ERROR_PHYSICS_INIT;
        }
        if (!world_.Init(math::Rectangle2D(world_size), &pool_quad_tree_)) {
            dt::Console(dt::FATAL, "Physicsengine::Init(): QuadTree::Init() failed");
            return Error::ERROR_PHYSICS_INIT;
        }

        /* The maximum number of collisions that will notify the objects is 128 */
        collisions_.Init(128);
        
        is_inited_ = true;
        return 0;
    }

    int PhysicsEngine::Destroy() {

        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* 
            TODO We have to iterate through all the PhysicsObjects held, and call their Destroy(),
            because Collision objects will be leaked
            Or else, we could offer custom allocation to such objects, and delete them alltogether here,
            like we do below for the quad tree
        */
        pool_quad_tree_.Destroy();

        is_inited_ = false;
        return 0;
    }

    bool PhysicsEngine::IsInited() {
        return is_inited_;
    }

    void PhysicsEngine::Step() {
        
        /* ... */

        /* ... */

        CallCollisionHandlers();
    }

    int PhysicsEngine::Insert(PhysicsObject * object) {
        bool ret = world_.Insert(math::Point2D(object->GetX(), object->GetY()), object);
        if (!ret) return Error::ERROR_OUT_OF_REGION;
        return 0;
    }

    int PhysicsEngine::Update(PhysicsObject * object, float new_pos_x, float new_pos_y) {
        bool ret = world_.Update(math::Point2D(object->GetX(), object->GetY()), object, math::Point2D(new_pos_x, new_pos_y));
        if (!ret) dt::Console(dt::WARNING, "PhysicsEngine::Update(): object not found");
        
        return 0;
    }

    void PhysicsEngine::Remove(PhysicsObject * object) {
        bool ret = world_.Remove(math::Point2D(object->GetX(), object->GetY()), object);
        if (!ret) dt::Console(dt::WARNING, "PhysicsEngine::Remove(): object not found");

        return;
    }

    size_t PhysicsEngine::GetObjectsArea(math::Rectangle2D search_area, std::vector<PhysicsObject*>& objects) {

        size_t nof = world_.QueryRange(search_area, objects);

        return nof;
    }

    CollisionResult_t PhysicsEngine::CheckCollision(PhysicsObject * object, math::Point2D new_position) {
        CollisionResult_t collision_res;

        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "PhysicsEngine::CheckCollision(): World sector is not initialised");
            return collision_res;
        }
        if (object == nullptr) {
            dt::Console(dt::CRITICAL, "PhysicsEngine::CheckCollision(): moving object is null");
            return collision_res;
        }
        if (!object->IsInited()) {
            dt::Console(dt::CRITICAL, "PhysicsEngine::CheckCollision(): moving object is not initialised");
            return collision_res;
        }

        collision_res.horizontal_ = CollisionGetDistance(object, { new_position.x_, object->GetY() }).first;
        collision_res.vertical_ = CollisionGetDistance(object, { object->GetX(), new_position.y_ }).second;

        return collision_res;
    }

    std::pair<float, float> PhysicsEngine::CollisionGetDistance(PhysicsObject * object, math::Point2D new_position) {
        /* Get neighbours */
        std::vector<PhysicsObject *> neighbours(50);
        size_t nof = world_.QueryRange(math::Rectangle2D(object->GetX(), object->GetY(), 3, 3), neighbours);
        
        ge::Real_t horizontal_move_offset = new_position.x_ - object->GetX();
        ge::Real_t vertical_move_offset = new_position.y_ - object->GetY();

        for (size_t i = 0; i < nof; i++) {
            PhysicsObject * neighbour = neighbours[i];

            if (object->Collides(new_position, neighbour)) {
                collisions_.Push(collision_detected_t(object, neighbour));

                /* TODO apply some clever mechanism to calculate remaining distance to colliding object */
                return std::pair<float, float>(0.0f, 0.0f);
            }
        }

        return std::pair<float, float>(horizontal_move_offset, vertical_move_offset);
    }

    void PhysicsEngine::CallCollisionHandlers() {
        for (size_t i = 0; i < collisions_.Items(); i++) {
            collision_detected_t t;
            collisions_.Get(t);

            /* If the colliding objects are still alive, then inform them of the collision */
            if (t.object1_->IsInited()) t.object1_->OnCollisionDetected(t.object2_->object_type_);
            if (t.object2_->IsInited()) t.object2_->OnCollisionDetected(t.object1_->object_type_);
        }
    }

}
}