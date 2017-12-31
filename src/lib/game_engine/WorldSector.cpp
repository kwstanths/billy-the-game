#include "WorldSector.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    WorldSector::WorldSector() {
        is_inited_ = false;
    }

    int WorldSector::Init(size_t width, size_t height, 
        float x_margin_start, float x_margin_end,
        float y_margin_start, float y_margin_end,
        size_t elements) 
    {
        world_ = std::vector<std::vector<std::deque<WorldObject *> > >(height, std::vector<std::deque<WorldObject *> >(width));
        objects_ = std::vector<WorldObject>(elements);
        sector_id_ = 0;

        x_margin_start_ = x_margin_start;
        x_margin_end_ = x_margin_end;
        y_margin_start_ = y_margin_start;
        y_margin_end_ = y_margin_end;

        is_inited_ = true;
        return 0;
    }

    int WorldSector::Destroy() {
        world_.clear();

        is_inited_ = false;
        return 0;
    }

    WorldObject * WorldSector::NewObj(float x, float y, float z) {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "World sector is not initialised");
            return nullptr;
        }
        if (sector_id_ >= objects_.size()) {
            dt::Console(dt::CRITICAL, "World sector's objects size has reached its limit");
            return nullptr;
        }

        objects_[sector_id_].SetPosition(x, y, z);
        WorldObject * the_new_object = &objects_[sector_id_];
        sector_id_++;

        size_t index_row = GetColumn(y);
        size_t index_col = GetRow(x);

        world_[index_row][index_col].push_back(the_new_object);

        return the_new_object;
    }

    size_t WorldSector::GetObjectsWindow(float center_x, float center_y, float margin, 
        std::vector<WorldObject*> & visible_world) 
    {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "World sector is not initialised");
            return -1;
        }

        /* TODO values sanitization*/
        size_t index = 0;
        size_t center_row = GetColumn(center_y);
        size_t center_col = GetRow(center_x);
        size_t border = std::ceil(margin);
        
        for (size_t i = center_row - border ; i <= center_row + border; i++) {
            for (size_t j = center_col - border; j <= center_col + border; j++) {
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

    bool WorldSector::CheckCollision(WorldObject * moving_object, float move_offset, size_t direction) {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): World sector is not initialised");
            return -1;
        }
        if (moving_object == nullptr) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): moving objectis null");
            return false;
        }
        if (!moving_object->IsInited()) {
            dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): moving objectis not initialised");
            return false;
        }


        /* Find the moving object's position inside the world. TODO out of boundaries */
        size_t moving_object_row_index = GetRow(moving_object->GetYPosition());
        if (moving_object_row_index >= world_.size() || moving_object_row_index < 0) {
            dt::ConsoleInfoL(dt::CRITICAL, "WorldSector::CheckCollision(): x index out of boundaries",
                "x index", moving_object_row_index);
            return false;
        }
        size_t moving_object_column_index = GetColumn(moving_object->GetXPosition());
        if (moving_object_column_index >= world_.size() || moving_object_column_index < 0) {
            dt::ConsoleInfoL(dt::CRITICAL, "WorldSector::CheckCollision(): y index out of boundaries",
                "y_index", moving_object_column_index);
            return false;
        }
        
        /* Get the type of collision of the moving object */
        CollisionConfig_t moving_object_collision_config = moving_object->GetCollision();


        size_t row_center = moving_object_row_index;
        size_t column_center = moving_object_column_index;
        /* Check collision with neighbours only in the desired directiion */
        switch (direction)
        {
        case 0 /* Up */:
        {
            /* 
                Going up in the world means going to bigger y world coordinates, 
                which means going to a higher number of row index in the world sector
            */
            for (size_t i = row_center; i <= row_center + 1; i++) {
                for (std::deque<WorldObject *>::iterator itr = world_[i][column_center].begin();
                    itr != world_[i][column_center].end();
                    ++itr)
                {
                    WorldObject * neighbour = (*itr);
                    CollisionConfig_t neighbour_collision_config = neighbour->GetCollision();
                    if (neighbour_collision_config.type_ != CollisionType::COLLISION_NONE) {
                        /* If some neighbour on the top has a collision type, then check collision */
                        
                        CodeReminder("Collision, Distinguish between different collision types");

                        if (CollisionCheck2DRectangles(moving_object->GetXPosition(),
                            moving_object->GetYPosition() + move_offset,
                            moving_object_collision_config.parameter_,
                            moving_object_collision_config.parameter_,
                            neighbour->GetXPosition(),
                            neighbour->GetYPosition(),
                            neighbour_collision_config.parameter_,
                            neighbour_collision_config.parameter_)) 
                        {
                            return false;
                        }
                    }
                }
            }

            break;
        }
        case 1 /* Bottom */:
        {
            /*
                
            */
            for (size_t i = row_center; i >= row_center - 1; i--) {
                for (std::deque<WorldObject *>::iterator itr = world_[i][column_center].begin();
                    itr != world_[i][column_center].end();
                    ++itr) 
                {
                    WorldObject * neighbour = (*itr);
                    CollisionConfig_t neighbour_collision_config = neighbour->GetCollision();
                    if (neighbour_collision_config.type_ != CollisionType::COLLISION_NONE) {
                        /* If some neighbour on the top has a collision type, then check collision */

                        CodeReminder("Collision, Distinguish between different collision types");

                        if (CollisionCheck2DRectangles(moving_object->GetXPosition(),
                            moving_object->GetYPosition() - move_offset,
                            moving_object_collision_config.parameter_,
                            moving_object_collision_config.parameter_,
                            neighbour->GetXPosition(),
                            neighbour->GetYPosition(),
                            neighbour_collision_config.parameter_,
                            neighbour_collision_config.parameter_))
                        {
                            return false;
                        }
                    }

                }
            }


            break;
        }
        case 2 /* Left */:
        {
            break;
        }
        case 3 /* Right */:
        {
            break;
        }
        default:
            dt::Console(dt::WARNING, "WorldSector::CheckCollision(): Unrecognised direction");
            return false;
        }


        return true;
    }

    size_t WorldSector::GetRow(float x) {
        return 0.0 + (world_[0].size()-1 - 0.0) * (x - x_margin_start_) / (x_margin_end_ - x_margin_start_);
    }

    size_t WorldSector::GetColumn(float y) {
        return 0.0 + (world_.size() - 1 - 0.0) * (y - y_margin_start_) / (y_margin_end_ - y_margin_start_);
    }

}

