#include "WorldObject.hpp"
#include "WorldSector.hpp"

#include "game_engine/physics/Collision.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"
#include "debug_tools/Assert.hpp"
namespace dt = debug_tools;
namespace ph = game_engine::physics;
namespace gl = game_engine::graphics::opengl;
namespace grph = game_engine::graphics;

//#define PHYSICS_ENGINE_DEBUG

namespace game_engine {

    WorldObject::WorldObject() : ph::PhysicsObject(), grph::GraphicsObject() {

        is_inited_ = false;
    }

    int WorldObject::Init(std::string model_file_path, Real_t x, Real_t y, Real_t z, bool interactable) {
        
        if (WorldObject::is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* Initialize the physics layer */
        int ret = PhysicsObject::Init(x, y, z);
        if (ret) {
            PrintError(ret);
            return ret;
        }

        /* Initialize the graphics layer */
        ret = GraphicsObject::Init(x, y, z, model_file_path);
        if (ret) {
            PrintError(ret);
            return ret;
        }

        interactable_ = interactable;

        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        PhysicsObject::Destroy();
        world_sector_->GetPhysicsEngine()->Remove(this);

        GraphicsObject::Destroy();

        world_sector_->RemoveObjectFromWorldStructure(this);
        world_sector_->DeleteObj(this);

        is_inited_ = false;
        return 0;
    }

    bool WorldObject::IsInited() {
        return is_inited_;
    }

    void WorldObject::Draw(grph::Renderer * renderer) {
        if (!is_inited_) return;

        /* Physice engine debugging */
#ifdef PHYSICS_ENGINE_DEBUG
        math::Shape2D * shape = GetCollision()->GetShape();
        math::Rectangle2D * brect = dynamic_cast<math::Rectangle2D *>(shape);
        if (brect != nullptr) {
            renderer->DrawRectangleXY(*brect, GetZ() + 0.01, 0.02, { 1,0,0 });
        }
#endif

        GraphicsObject::Draw(renderer);
    }

    void WorldObject::Step(double delta_time) {
        /* Re-implement this function for custom behaviour at every frame */
    }

    void WorldObject::Interact() {
        /* Re-implement this function for custom interact behaviour */
    }

    void WorldObject::SetPosition(Real_t pos_x, Real_t pos_y, Real_t pos_z) {

        /* Maybe not assertion but return something */
        _assert(world_sector_ != nullptr)

        /* Update the object's position inside the world sector */
        world_sector_->UpdateObjectPosition(this, GetX(), GetY(), pos_x, pos_y);
        
        /* Set the position in the physics layer */
        world_sector_->GetPhysicsEngine()->Update(this, math::Point2D(pos_x, pos_y));
        PhysicsObject::SetPosition(pos_x, pos_y, pos_z);
        
        /* Set the position in the graphics layer */
        GraphicsObject::SetPosition(pos_x, pos_y, pos_z);
    }

    bool WorldObject::SetPosition(Real_t pos_x, Real_t pos_y, Real_t pos_z, math::Rectangle2D collision_area_check) {
        
        Real_t new_pos_x = pos_x;
        Real_t new_pos_y = pos_y;

        ph::PhysicsEngine * physics_engine = world_sector_->GetPhysicsEngine();
        ph::CollisionResult_t collision_result = physics_engine->CheckCollision(this, { pos_x, pos_y }, collision_area_check);
        
        new_pos_x = GetX() + collision_result.horizontal_;
        new_pos_y = GetY() + collision_result.vertical_;
    
        SetPosition(new_pos_x, new_pos_y, pos_z);

        return !(math::Equal(pos_x, new_pos_x) && math::Equal(pos_y, new_pos_y));
    }

    void WorldObject::Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z) {
        if (!(math::Equal(scale_x, scale_y) && math::Equal(scale_y, scale_z))) dt::Console(dt::WARNING, "Non uniform scale");
        
        /* TODO Maybe scale in the physics layer as well? */
    
        GraphicsObject::Scale(scale_x, scale_y, scale_z);
    }

    void WorldObject::Rotate(Real_t angle, glm::vec3 axis) {        
        
        if (axis == glm::vec3(0, 0, 1)) {
            /* Update physics object */
            /*
                Currently, rotation is done in a 2d manner only in the xy pane clockwise, i.e along the z-axis
                This means that positive angle will actually rotate along the -z axis, and
                negative angle will rotate along the z-axis. That's why we pass the opositve of the angle, since
                the GetRotateMatrix has opposite behaviour
            */
            PhysicsObject::Rotate(-angle);
        }

        GraphicsObject::Rotate(angle, axis);
    }

}
