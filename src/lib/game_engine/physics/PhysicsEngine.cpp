#include "PhysicsEngine.hpp"

#include "game_engine/memory/MemoryManager.hpp"

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

    int PhysicsEngine::Init(math::AABox<2> world_size, size_t number_of_objects) {

        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        float length = std::max(world_size.max_[0] - world_size.min_[0], world_size.max_[1] - world_size.min_[1]);
        world_ = new utility::QuadTree<PhysicsObject *>(world_size.min_, length);

        is_inited_ = true;
        return 0;
    }

    int PhysicsEngine::Destroy() {

        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* 
            TODO We have to iterate through all the PhysicsObjects held, and call their Destroy(),
            because Collision objects will be leaked
            Or else, we could offer custom allocation to such objects, and delete them alltogether here
        */

        is_inited_ = false;
        return 0;
    }

    bool PhysicsEngine::IsInited() {
        return is_inited_;
    }

    int PhysicsEngine::Insert(PhysicsObject * object, math::Point2D& position) {
        bool ret = world_->Insert(position, object);
        if (!ret) return Error::ERROR_OUT_OF_REGION;

        return 0;
    }

    int PhysicsEngine::Update(PhysicsObject * object, math::Point2D& new_position) {
        world_->Remove(math::Point2D({ object->GetX(), object->GetY() }));
        
        bool ret = world_->Insert(new_position, object);
        if (!ret) return Error::ERROR_OUT_OF_REGION;

        return 0;
    }

    void PhysicsEngine::Remove(PhysicsObject * object) {
        world_->Remove(math::Point2D({ object->GetX(), object->GetY() }));
    }

    void PhysicsEngine::GetObjectsArea(math::AABox<2> search_area, std::vector<PhysicsObject*>& objects) {
        world_->QueryRange(search_area, objects);
    }

    math::Point2D PhysicsEngine::CheckCollision(PhysicsObject * object, math::Point2D new_position) {

        math::Point2D result = Point2D({ object->GetX(), object->GetY() });

        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "PhysicsEngine::CheckCollision(): World sector is not initialised");
            return result;
        }
        if (object == nullptr) {
            dt::Console(dt::CRITICAL, "PhysicsEngine::CheckCollision(): moving object is null");
            return result;
        }
        if (!object->IsInited()) {
            dt::Console(dt::CRITICAL, "PhysicsEngine::CheckCollision(): moving object is not initialised");
            return result;
        }

        /* Get neighbours around a small area */
        std::vector<PhysicsObject *> neighbours;
        math::Point2D object_position = Point2D({ object->GetX(), object->GetY() });
        world_->QueryRange(math::AABox<2>(object_position - 1.5, object_position + 1.5), neighbours);

        /* Calculate offset between new and old position without collision */
        math::Point2D offset = math::Point2D({ new_position.x() - object->GetX(), new_position.y() - object->GetY() });

        /* Check separately in horizontal and vertical directions for collision, and set that offset to zero */
        for (size_t i = 0; i < neighbours.size(); i++) {
            PhysicsObject * neighbour = neighbours[i];

            if (object->Collides(Point2D({ new_position.x(), object->GetY() }), neighbour)) {
                offset[0] = 0;
            }
            if (object->Collides(Point2D({ object->GetX(), new_position.y() }), neighbour)) {
                offset[1] = 0;
            }
        }

        return result + offset;
    }

}
}