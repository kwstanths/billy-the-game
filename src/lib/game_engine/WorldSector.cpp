#include "WorldSector.hpp"
#include "GameEngine.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "ErrorCodes.hpp"
#include "Collision.hpp"
#include "physics/HelpFunctions.hpp"
#include "physics/Types.hpp"

namespace dt = debug_tools;
namespace ms = game_engine::memory_subsystem;

namespace game_engine {

    WorldSector::WorldSector() {
        is_inited_ = false;
    }

    int WorldSector::Init(size_t width, size_t height, 
        float x_margin_start, float x_margin_end,
        float y_margin_start, float y_margin_end,
        size_t elements, GameEngine * engine) 
    {
        world_ = std::vector<std::vector<std::deque<WorldObject *> > >(height, std::vector<std::deque<WorldObject *> >(width));
        array_objects_ = new ms::ArrayAllocator();
        array_objects_->Init(500 * 500);

        x_margin_start_ = x_margin_start;
        x_margin_end_ = x_margin_end;
        y_margin_start_ = y_margin_start;
        y_margin_end_ = y_margin_end;

        if (engine == nullptr) return Error::ERROR_ENGINE_NOT_INIT;
        if (!engine->IsInited()) return Error::ERROR_ENGINE_NOT_INIT;
        engine_ = engine;

        is_inited_ = true;
        return 0;
    }

    int WorldSector::Destroy() {
        world_.clear();
         
        is_inited_ = false;
        return 0;
    }

    bool WorldSector::IsInited() {
        return is_inited_;
    }

    size_t WorldSector::GetObjectsWindow(float center_x, float center_y, float margin, 
        std::vector<WorldObject*> & visible_world) 
    {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::GetObjectsWindow() is not initialised");
            return -1;
        }

        size_t index = 0;
        size_t center_row = GetRow(center_y);
        size_t center_col = GetColumn(center_x);
        size_t border = static_cast<size_t>(std::ceil(margin));
        
        size_t rows_start = (center_row - border >= 0) ? (center_row - border) : 0;
        size_t rows_end = (center_row + border < world_.size()) ? (center_row + border) : (world_.size() - 1);
        size_t col_start = (center_col - border >= 0) ? (center_col - border) : 0;
        size_t col_end = (center_col + border < world_[0].size()) ? (center_col + border) : (world_[0].size());

        for (size_t i = rows_start; i <= rows_end; i++) {
            for (size_t j = col_start - border; j <= col_end; j++) {
                for (std::deque<WorldObject*>::iterator itr = world_[i][j].begin(); itr != world_[i][j].end(); ++itr) {
                    if (index > visible_world.size() - 1) {
                        dt::Console(dt::WARNING, "WorldSector::GetObjetcsWindow() Objects overflow");
                        return index;
                    }
                    visible_world[index++] = *itr;
                }

            }
        }
        return index;
    }

    CollisionResult_t WorldSector::CheckCollision(WorldObject * moving_object, float move_offset, Direction direction) {
        CollisionResult_t collision_res;

        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): World sector is not initialised");
            return collision_res;
        }
        if (moving_object == nullptr) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): moving objectis null");
            return collision_res;
        }
        if (!moving_object->IsInited()) {
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

        Point2D moving_object_new_position(moving_object->GetXPosition(), moving_object->GetYPosition());
        if (moving_top) {
            moving_object_new_position.y_ += move_offset;
            collision_res.up_ = CollisionGetDistance(moving_object, moving_object_new_position).first;
            moving_object_new_position.y_ = moving_object->GetYPosition() + collision_res.up_;
        }
        else if (moving_down) {
            moving_object_new_position.y_ -= move_offset;
            collision_res.down_ = CollisionGetDistance(moving_object, moving_object_new_position).first;
            moving_object_new_position.y_ = moving_object->GetYPosition() - collision_res.up_;
        }

        if (moving_left) {
            moving_object_new_position.x_ -= move_offset;
            collision_res.left_ = CollisionGetDistance(moving_object, moving_object_new_position).second;
            moving_object_new_position.x_ = moving_object->GetXPosition() - collision_res.left_;

        } else if (moving_right) {
            moving_object_new_position.x_ += move_offset;
            collision_res.right_ = CollisionGetDistance(moving_object, moving_object_new_position).second;
            moving_object_new_position.x_ = moving_object->GetXPosition() - collision_res.right_;

        }
        
        return collision_res;
    }

    size_t WorldSector::GetRow(float x) {
        float index = 0.0f + (world_[0].size() - 1 - 0.0f) * (x - x_margin_start_) / (x_margin_end_ - x_margin_start_);
        return static_cast<size_t>(index);
    }

    size_t WorldSector::GetColumn(float y) {
        float index = 0.0f + (world_.size() - 1 - 0.0f) * (y - y_margin_start_) / (y_margin_end_ - y_margin_start_);
        return static_cast<size_t>(index);
    }

    std::pair<float, float> WorldSector::CollisionGetDistance(WorldObject * moving_object, Point2D new_position) {
        /* Get neighbours */
        std::vector<WorldObject *> neighbours(50);
        size_t nof = GetObjectsWindow(moving_object->GetXPosition(), moving_object->GetYPosition(), 1, neighbours);
        
        float horizontal_move_offset = std::abs(moving_object->GetXPosition() - new_position.x_);
        float vertical_move_offset = std::abs(moving_object->GetYPosition() - new_position.y_);
        
        for (size_t i = 0; i < nof; i++) {
            WorldObject * neighbour = neighbours[i];

            if (moving_object->Collides(new_position, neighbour)) {
                /* TODO apply some clever mechanism to calculate remaining distance to colliding object */
                return std::pair<float, float>(0.0f, 0.0f);
            }
        }

        return std::pair<float, float>(vertical_move_offset, horizontal_move_offset);
    }

}

