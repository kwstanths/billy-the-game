#include "WorldObject.hpp"

#include "physics/Matrices.hpp"
#include "WorldSector.hpp"
#include "ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace ph = game_engine::physics;

namespace game_engine {

    WorldObject::WorldObject() : ph::PhysicsObject() {

        translation_matrix_ = GetTranslateMatrix(GetX(), GetY(), GetZ());

        is_inited_ = false;
    }

    int WorldObject::Init(OpenGLObject * object, OpenGLTexture * texture, float x, float y, float z) {
        
        if (WorldObject::is_inited_) return Error::ERROR_GEN_NOT_INIT;

        int ret = PhysicsObject::Init(x, y, z, world_sector_->GetPhysicsEngine());
        if (ret) {
            PrintError(ret);
            return ret;
        }

        if (object == nullptr) {
            PrintError(Error::ERROR_OBJECT_NOT_INIT);
            return Error::ERROR_OBJECT_NOT_INIT;
        }

        if (texture == nullptr) {
            PrintError(Error::ERROR_TEXTURE_NOT_INIT);
            return Error::ERROR_TEXTURE_NOT_INIT;
        }

        if (!object->IsInited()) {
            PrintError(Error::ERROR_OBJECT_NOT_INIT);
            return Error::ERROR_OBJECT_NOT_INIT;
        }
        if (!texture->IsInited()) {
            PrintError(Error::ERROR_TEXTURE_NOT_INIT);
            return Error::ERROR_TEXTURE_NOT_INIT;
        }

        if (world_sector_ != nullptr) world_sector_->Insert(this, x, y, z);
        translation_matrix_ = GetTranslateMatrix(GetX(), GetY(), GetZ());

        scale_matrix_ = GetScaleMatrix(1.0f, 1.0f, 1.0f);
        rotated_angle_ = 0.0f;

        object_ = object;
        texture_ = texture;

        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        CodeReminder("TODO Remove from the world sector");
        CodeReminder("TODO Call the PhysicsObject::Destroy()");
        
        /* 
            DON'T delete OpenGLObject and OpenGLTexture pointers !!! 
            They might be used by other WorldObjects
        */
        CodeReminder("TODO Change These pointers into something else, like shared_ptr for example");

        is_inited_ = false;
        return 0;
    }

    bool WorldObject::IsInited() {
        return is_inited_;
    }

    void WorldObject::Draw(OpenGLRenderer * renderer) {
        if (!is_inited_) return;

        model_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
        renderer->Draw(object_, texture_, model_);
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
        
        /* Set the translation matrix */
        translation_matrix_= GetTranslateMatrix(GetX(), GetY(), GetZ());
    }

    void WorldObject::Scale(float scale_x, float scale_y, float scale_z) {
        /* TODO Maybe change collision detecton as well */
        scale_matrix_ = GetScaleMatrix(scale_x, scale_y, scale_z);
    }

    void WorldObject::Rotate(float angle, size_t axis) {        
        glm::vec3 rotation_axis;

        if (axis == 0) rotation_matrix_ = GetRotateMatrix(angle, 1, 0, 0);
        else if (axis == 1) rotation_matrix_ = GetRotateMatrix(angle, 0, 1, 0);
        else if (axis == 2) rotation_matrix_ = GetRotateMatrix(angle, 0, 0, 1);
        
        rotated_angle_ = angle;

        /* Update physics object */
        /* 
            Currently, rotation is done in a 2d manner only in the xy pane clockwise, i.e along the z-axis
            This means that positive angle will actually rotate along the -z axis, and 
            negative angle will rotate along the z-axis. That's why we pass the opositve of the angle, since
            the GetRotateMatrix has opposite behaviour 
        */
        PhysicsObject::Rotate(-angle);
    }

    Direction WorldObject::GetLookingDirection() {
        return  rotated_angle_;
    }

}
