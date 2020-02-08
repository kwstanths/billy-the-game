#include "WorldObject.hpp"
#include "WorldSector.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"
#include "debug_tools/Assert.hpp"
namespace dt = debug_tools;
namespace gl = game_engine::graphics::opengl;
namespace grph = game_engine::graphics;


namespace game_engine {

    WorldObject::WorldObject() : grph::GraphicsObject() {

        is_inited_ = false;
    }

    int WorldObject::Init() {
        
        if (WorldObject::is_inited_) return Error::ERROR_GEN_NOT_INIT;

        is_inited_ = true;
        return 0;
    }

    int WorldObject::Init(Real_t x, Real_t y, Real_t z) {
        if (WorldObject::is_inited_) return Error::ERROR_GEN_NOT_INIT;

        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
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
        _assert(world_sector_ != nullptr);

        /* Update the object's position inside the world sector */
        world_sector_->UpdateObjectInWorldStructure(this, position_[0], position_[2], pos_x, pos_z);
        
        /* Set the position in the graphics layer */
        GraphicsObject::SetPosition(pos_x, pos_y, pos_z);
        
        position_ = math::Vector3D({ pos_x, pos_y, pos_z });
    }

    void WorldObject::Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z) {
        //if (!(math::Equal(scale_x, scale_y) && math::Equal(scale_y, scale_z))) dt::Console(dt::WARNING, "Non uniform scale");
        
        GraphicsObject::Scale(scale_x, scale_y, scale_z);
    }

    void WorldObject::Rotate(Real_t angle, glm::vec3 axis) {        
        
        GraphicsObject::Rotate(angle, axis);
    }

    Real_t WorldObject::GetX() {
        return position_[0];
    }

    Real_t WorldObject::GetY() {
        return position_[1];
    }

    Real_t WorldObject::GetZ() {
        return position_[2];
    }

    math::Vector3D WorldObject::GetPosition() {
        return position_;
    }

}
