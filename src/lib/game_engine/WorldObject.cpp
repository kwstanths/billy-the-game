#include "WorldObject.hpp"

#include "physics/Matrices.hpp"
#include "WorldSector.hpp"
#include "ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    WorldObject::WorldObject() {

        /* Set at position 0 with no collision */
        SetCollision();
        
        pos_x_ = 0;
        pos_y_ = 0;
        pos_z_ = 0;
        translation_matrix_ = GetTranslateMatrix(pos_x_, pos_y_, pos_z_);

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

    float WorldObject::GetXPosition() {
        return pos_x_;
    }

    float WorldObject::GetYPosition() {
        return pos_y_;
    }

    float WorldObject::GetZPosition() {
        return pos_z_;
    }

    void WorldObject::SetPosition(float pos_x, float pos_y, float pos_z) {
        
        collision_->Translate(pos_x - pos_x_, pos_y - pos_y_);

        /* 
            This order matters since UpdateObjectPosition will ask this object fro it's old position
            If the assignment is done before the call, then new_pos and old_pos will have the same values
        */
        world_sector_->UpdateObjectPosition(this, pos_x, pos_y);
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;

        translation_matrix_= GetTranslateMatrix(pos_x_, pos_y_, pos_z_);

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

    void WorldObject::SetCollision() {
        collision_type_ = CollisionType::COLLISION_NONE;
        collision_ = new CollisionNone();
    }

    void WorldObject::SetCollision(float x_size, float y_size, float z_size) {
        collision_type_ = CollisionType::COLLISION_BOUNDING_RECTANGLE;
        collision_ = new CollisionBoundingRectangle(Rectangle2D(pos_x_, pos_y_, x_size, y_size));
    }

    void WorldObject::SetCollision(float radius) {
        collision_type_ = CollisionType::COLLISION_BOUNDING_CIRCLE;
        collision_ = new CollisionBoundingCircle(Circle2D(pos_x_, pos_y_, radius));
    }

    CollisionType WorldObject::GetCollisionType() {
        return collision_type_;
    }

    Collision * WorldObject::GetCollision() {
        return collision_;
    }

    bool WorldObject::Collides(Point2D new_position, WorldObject * other) {

        if (!is_inited_) return false;

        /* Apply rotations if applicable */
        CollisionType neighbour_collision_type = other->GetCollisionType();
        CollisionType moving_object_collision_type = collision_type_;

        /* We do not collide with ourselves */
        if (other == this) return false;

        if (neighbour_collision_type == CollisionType::COLLISION_NONE) return false;
        if (moving_object_collision_type == CollisionType::COLLISION_NONE) return false;
        
        /* Move the collision object to new position */
        float x_offset = new_position.x_ - pos_x_;
        float y_offset = new_position.y_ - pos_y_;
        collision_->Translate(x_offset, y_offset);
        Collision * neighbour_collision = other->GetCollision();

        /* Check collision */
        bool collides = neighbour_collision->Check(collision_);
        
        /* Move collision object back */
        collision_->Translate(-x_offset, -y_offset);
        
        return collides;
    }

}
