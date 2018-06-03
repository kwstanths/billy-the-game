#include "WorldObject.hpp"

#include "physics/Matrices.hpp"
#include "WorldSector.hpp"
#include "ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace ph = game_engine::physics;

namespace game_engine {

    WorldObject::WorldObject() : ph::PhysicsObject() {

        translation_matrix_ = GetTranslateMatrix(GetX(), GetY(), GetZ());

        is_inited_ = false;
    }

    int WorldObject::Init(OpenGLObject * object, OpenGLTexture * texture) {
         
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

        scale_matrix_ = GetScaleMatrix(1.0f, 1.0f, 1.0f);
        rotated_angle_ = 0.0f;

        object_ = object;
        texture_ = texture;

        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        

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
        
    }

    void WorldObject::SetPosition(float pos_x, float pos_y, float pos_z) {
        world_sector_->UpdateObjectPosition(this, GetX(), GetY(), pos_x, pos_y);
        
        PhysicsObject::SetPosition(pos_x, pos_y, pos_z);
        
        translation_matrix_= GetTranslateMatrix(GetX(), GetY(), GetZ());
    }

    void WorldObject::Scale(float scale_x, float scale_y, float scale_z) {
        scale_matrix_ = GetScaleMatrix(scale_x, scale_y, scale_z);
    }

    void WorldObject::Rotate(float angle, size_t axis) {        
        glm::vec3 rotation_axis;

        if (axis == 0) rotation_matrix_ = GetRotateMatrix(angle, 1, 0, 0);
        else if (axis == 1) rotation_matrix_ = GetRotateMatrix(angle, 0, 1, 0);
        else if (axis == 2) rotation_matrix_ = GetRotateMatrix(angle, 0, 0, 1);
        
        rotated_angle_ = angle;
    }

}
