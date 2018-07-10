#include "WorldObject.hpp"
#include "WorldSector.hpp"

#include "ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace ph = game_engine::physics;
namespace gl = game_engine::graphics::opengl;
namespace grph = game_engine::graphics;

namespace game_engine {

    WorldObject::WorldObject() : ph::PhysicsObject(), grph::GraphicsObject() {

        is_inited_ = false;
    }

    int WorldObject::Init(std::string model_file_path, float x, float y, float z, bool interactable) {
        
        if (WorldObject::is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* Initialize the physics layer */
        int ret = PhysicsObject::Init(x, y, z, world_sector_->GetPhysicsEngine());
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

        /* Insert in the world */
        if (world_sector_ != nullptr) world_sector_->Insert(this, x, y, z);

        rotated_angle_ = 0.0f;
        interactable_ = interactable;

        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        PhysicsObject::Destroy();

        GraphicsObject::Destroy();

        world_sector_->Remove(this);

        is_inited_ = false;
        return 0;
    }

    bool WorldObject::IsInited() {
        return is_inited_;
    }

    void WorldObject::Draw(grph::Renderer * renderer) {
        if (!is_inited_) return;
        /* Is this function useless now? */
        GraphicsObject::Draw(renderer);
    }

    void WorldObject::Step(double delta_time) {
        /* Re-implement this function for custom behaviour at every frame */
    }

    void WorldObject::Interact() {
        /* Re-implement this function for custom interact behaviour */
    }

    void WorldObject::SetPosition(float pos_x, float pos_y, float pos_z) {
        /* Update the object's position inside the world sector */
        world_sector_->UpdateObjectPosition(this, GetX(), GetY(), pos_x, pos_y);
        
        /* Set the position in the physics layer */
        PhysicsObject::SetPosition(pos_x, pos_y, pos_z);
        
        /* Set the position in the graphics layer */
        GraphicsObject::SetPosition(pos_x, pos_y, pos_z);
    }

    void WorldObject::Scale(float scale_x, float scale_y, float scale_z) {
        if (!(Equal(scale_x, scale_y) && Equal(scale_y, scale_z))) dt::Console(dt::WARNING, "Non uniform scale");
        
        /* TODO Maybe scale in the physics layer as well? */
    
        GraphicsObject::Scale(scale_x, scale_y, scale_z);
    }

    void WorldObject::Rotate(float angle, size_t axis) {        
        
        rotated_angle_ = angle;

        /* Update physics object */
        /* 
            Currently, rotation is done in a 2d manner only in the xy pane clockwise, i.e along the z-axis
            This means that positive angle will actually rotate along the -z axis, and 
            negative angle will rotate along the z-axis. That's why we pass the opositve of the angle, since
            the GetRotateMatrix has opposite behaviour 
        */
        PhysicsObject::Rotate(-angle);

        GraphicsObject::Rotate(angle, axis);
    }

    Direction WorldObject::GetLookingDirection() {
        return  rotated_angle_;
    }

}
