#include "WorldObject.hpp"

#include "physics/Matrices.hpp"
#include "ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    WorldObject::WorldObject() {

        SetPosition(0.0f, 0.0f, 0.0f);

        SetCollision();
        rotated_angle_ = 0.0f;

        is_inited_ = false;
    }

    int WorldObject::Init(OpenGLObject * object, OpenGLTexture * texture, OpenGLRenderer * renderer) {
         
        if (!object->IsInited()) {
            PrintError(Error::ERROR_OBJECT_NOT_INIT);
            return Error::ERROR_OBJECT_NOT_INIT;
        }
        if (!texture->IsInited()) {
            PrintError(Error::ERROR_TEXTURE_NOT_INIT);
            return Error::ERROR_TEXTURE_NOT_INIT;
        }

        object_ = object;
        texture_ = texture;
        renderer_ = renderer;
        
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

    void WorldObject::Draw() {
        if (!is_inited_) return;

        model_ = translation_matrix_ * rotation_matrix_;
        renderer_->Draw(object_, texture_, model_);
    }

    void WorldObject::Step(double delta_time) {
        
    }

    float WorldObject::GetXPosition() {
        return pos_x_;
    }

    float WorldObject::GetYPosition() {
        return pos_y_;
    }

    float WorldObject::GetZPosition() {
        return pos_z_;
    }

    float WorldObject::GetObjectWidth() {
        return size_x_;
    }

    float WorldObject::GetObjectHeight() {
        return size_y_;
    }

    void WorldObject::SetPosition(float pos_x, float pos_y, float pos_z) {
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;

        translation_matrix_= GetTranslateMatrix(glm::vec3(pos_x_, pos_y_, pos_z_));

    }

    void WorldObject::Rotate(float angle, size_t axis) {        
        glm::vec3 rotation_axis;

        if (axis == 0) rotation_axis = glm::vec3(1, 0, 0);
        else if (axis == 1) rotation_axis = glm::vec3(0, 1, 0);
        else if (axis == 2) rotation_axis = glm::vec3(0, 0, 1);
        
        rotation_matrix_ = GetRotateMatrix(angle, rotation_axis);
        
        rotated_angle_ = angle;
    }

    void WorldObject::SetCollision() {
        collision_type_ = CollisionType::COLLISION_NONE;
        size_x_ = 0.0f;
        size_y_ = 0.0f;
        size_z_ = 0.0f;
    }

    void WorldObject::SetCollision(float x_size, float y_size, float z_size) {
        collision_type_ = CollisionType::COLLISION_BOUNDING_RECTANGLE;
        size_x_ = x_size;
        size_y_ = y_size;
        size_z_ = z_size;
    }

    void WorldObject::SetCollision(float radius) {
        collision_type_ = CollisionType::COLLISION_BOUNDING_CIRCLE;
        size_x_ = 2.0f*radius;
        size_y_ = 2.0f*radius;
        size_z_ = 2.0f*radius;
    }

    CollisionType WorldObject::GetCollisionType() {
        return collision_type_;
    }

    bool WorldObject::Collides(Point2D_t new_position, WorldObject * other) {

        if (!is_inited_) return false;
        
        /* Apply rotations if applicable */
        CollisionType neighbour_collision_type = other->GetCollisionType();
        CollisionType moving_object_collision_type = collision_type_;

        if (neighbour_collision_type == CollisionType::COLLISION_NONE) return false;
        if (moving_object_collision_type == CollisionType::COLLISION_NONE) return false;

        if (moving_object_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
            Rectangle2D_t mo_br(new_position.x_, new_position.y_, size_x_, size_y_);
            
            if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
                Circle2D_t n_bc(other->GetXPosition(), other->GetYPosition(), other->GetObjectWidth() / 2.0f);
                return CollisionCheck(mo_br, n_bc);
            } else if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
                Rectangle2D_t n_br(other->GetXPosition(), other->GetYPosition(), other->GetObjectWidth(), other->GetObjectHeight());
                return CollisionCheck(mo_br, n_br);
            }

        } else if (moving_object_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
            Circle2D_t mo_bc(new_position.x_, new_position.y_, size_x_ / 2.0f);
            
            if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
                Circle2D_t n_bc(other->GetXPosition(), other->GetYPosition(), other->GetObjectWidth() / 2.0f);
                return CollisionCheck(mo_bc, n_bc);
            } else if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
                Rectangle2D_t n_br(other->GetXPosition(), other->GetYPosition(), other->GetObjectWidth(), other->GetObjectHeight());
                return CollisionCheck(n_br, mo_bc);
            }
        }
        
        return false;
    }

}
