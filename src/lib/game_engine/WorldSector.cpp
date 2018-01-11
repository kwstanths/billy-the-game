#include "WorldSector.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "Collision.hpp"
#include "HelpFunctions.hpp"
#include "physics/Types.hpp"

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
            dt::Console(dt::CRITICAL, "WorldSector::GetObjectsWindow() is not initialised");
            return -1;
        }

        size_t index = 0;
        size_t center_row = GetRow(center_y);
        size_t center_col = GetColumn(center_x);
        size_t border = std::ceil(margin);
        
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

        float moving_object_new_x = moving_object->GetXPosition();
        float moving_object_new_y = moving_object->GetYPosition();

        if (Equal(direction, 0.0f)) {
            moving_object_new_y += move_offset;
            collision_res.up_ = move_offset;
        } else if (Equal(direction, 90.0f)) {
            moving_object_new_x -= move_offset;
            collision_res.left_ = move_offset;
        } else if (Equal(direction, 180.0f)) {
            moving_object_new_y -= move_offset;
            collision_res.down_ = move_offset;
        } else if (Equal(direction, 270.0f)) {
            moving_object_new_x += move_offset;
            collision_res.right_ = move_offset;
        } else if (direction < 90.0f){
            moving_object_new_x -= move_offset;
            moving_object_new_y += move_offset;
            collision_res.left_ = move_offset;
            collision_res.up_ = move_offset;
        } else if (direction < 180.0f) {
            moving_object_new_x -= move_offset;
            moving_object_new_y -= move_offset;
            collision_res.left_ = move_offset;
            collision_res.down_ = move_offset;
        } else if (direction < 270.0f) {
            moving_object_new_x += move_offset;
            moving_object_new_y -= move_offset;
            collision_res.right_ = move_offset;
            collision_res.down_ = move_offset;
        } else {
            moving_object_new_x += move_offset;
            moving_object_new_y += move_offset;
            collision_res.right_ = move_offset;
            collision_res.up_ = move_offset;
        }

        std::vector<WorldObject *> neighbours(50);
        size_t nof = GetObjectsWindow(moving_object->GetXPosition(), moving_object->GetYPosition(), 1, neighbours);
        for (size_t i = 0; i < nof; i++) {
            WorldObject * neighbour = neighbours[i];
            CollisionType neighbour_collision_type = neighbour->GetCollisionType();
            if (neighbour_collision_type != CollisionType::COLLISION_NONE) {

                if (!Equal(collision_res.up_, 0.0f)) {
                    /* Check up */
                }
                if (!Equal(collision_res.down_, 0.0f)) {
                    /* Check down */
                }
                if (!Equal(collision_res.left_, 0.0f)) {
                    /* Check left */
                }
                if (!Equal(collision_res.right_, 0.0f)) {
                    /* Check right */
                }

                CollisionType moving_object_collision_type = moving_object->GetCollisionType();
                if (moving_object_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
                    if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
                        Rectangle2D_t mo_br(moving_object_new_x, moving_object_new_y, moving_object->GetObjectWidth(), moving_object->GetObjectHeight());
                        Circle2D_t n_bc(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth() / 2.0);
                      
                        CollisionCheck(mo_br, n_bc);
                    } else if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
                        Rectangle2D_t mo_br(moving_object_new_x, moving_object_new_y, moving_object->GetObjectWidth(), moving_object->GetObjectHeight());
                        Rectangle2D_t n_br(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth(), neighbour->GetObjectHeight());

                        if (CollisionCheck(mo_br, n_br)) return CollisionResult_t();
                    }
                }
                else if (moving_object_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
                    if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
                        Circle2D_t mo_bc(moving_object_new_x, moving_object_new_y, moving_object->GetObjectWidth() / 2.0);
                        Circle2D_t n_bc(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth() / 2.0);
                        
                        CollisionCheck(mo_bc, n_bc);
                    }
                    else if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
                        Circle2D_t mo_bc(moving_object_new_x, moving_object_new_y, moving_object->GetObjectWidth() / 2.0);
                        Rectangle2D_t n_br(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth(), neighbour->GetObjectHeight());
                        
                        CollisionCheck(n_br, mo_bc);
                    }
                }


            }
        }


















        ///* Find the moving object's position inside the world sector */
        //size_t object_row_center = GetRow(moving_object->GetYPosition());
        //size_t object_column_center = GetColumn(moving_object->GetXPosition());

        ///* Check collision with neighbours only in the desired directiion */
        //size_t rows_start = object_row_center;
        //size_t rows_end = object_row_center;
        //size_t columns_start = object_column_center;
        //size_t columns_end = object_column_center;
        //float moving_object_new_x;
        //float moving_object_new_y;

        //switch (direction)
        //{
        //case 0 /* Up */:
        //{

        //    if (object_column_center < 0 || object_column_center >= world_[0].size()) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Top] - Column center is out of boundaries");
        //        return 0.0f;
        //    }

        //    /*
        //        Going up in the world means going to bigger y world coordinates,
        //        which means going to a bigger number of row index in the world sector
        //    */
        //    if (object_row_center >= world_.size()) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Top] - Row center is bigger than the world rows");
        //        return 0.0f;
        //    }
        //    rows_start = (object_row_center < 0) ? 0 : object_row_center;
        //    rows_end = (rows_start + 1 >= world_.size()) ? world_.size() - 1 : rows_start + 1;
        //    columns_start = (object_column_center - 1 < 0) ? 0 : (object_column_center - 1);
        //    columns_end = (object_column_center + 1 >= world_[0].size()) ? world_[0].size() - 1 : (object_column_center + 1);

        //    moving_object_new_x = moving_object->GetXPosition();
        //    moving_object_new_y = moving_object->GetYPosition() + move_offset;

        //    break;
        //}
        //case 1 /* Bottom */:
        //{
        //    if (object_column_center < 0 || object_column_center >= world_[0].size()) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Bottom] - Column center is out of boundaries");
        //        return 0.0f;
        //    }

        //    /*
        //        Going down in the world means going to smaller y world coordinates,
        //        which means going to a smaller number of row in the world sector
        //    */
        //    if (object_row_center < 0) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Bottom] - Row center is smaller than zero");
        //        return 0.0f;
        //    }
        //    rows_start = (object_row_center >= world_.size()) ? world_.size() - 1 : object_row_center;
        //    rows_end = (rows_start - 1 < 0) ? 0 : rows_start - 1;
        //    columns_start = (object_column_center - 1 < 0) ? object_column_center : (object_column_center - 1);
        //    columns_end = (object_column_center + 1 >= world_[0].size()) ? world_[0].size() - 1 : (object_column_center + 1);

        //    /* Swap the values so that we always use ++ operator when iterating over the objects */
        //    Swap(rows_start, rows_end);
        //    
        //    moving_object_new_x = moving_object->GetXPosition();
        //    moving_object_new_y = moving_object->GetYPosition() - move_offset;

        //    break;
        //}
        //case 2 /* Left */:
        //{
        //    if (object_row_center < 0 || object_row_center >= world_.size()) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Left] - Row center is out of baoundaries");
        //        return 0.0f;
        //    }

        //    /*
        //        Going left in the world means going to smaller x world coordinates
        //        which  means going to a smaller number of column
        //    */
        //    if (object_column_center < 0) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Left] - Column center is smaller than zero");
        //        return 0.0f;
        //    }
        //    columns_start = (object_column_center >= world_[0].size()) ? world_[0].size() - 1 : object_column_center;
        //    columns_end = (columns_start - 1 < 0) ? 0 : columns_start - 1;
        //    rows_start = (object_row_center - 1 < 0) ? 0 : (object_row_center - 1);
        //    rows_end = (object_row_center + 1 >= world_.size()) ? world_.size() - 1 : (object_row_center + 1);
        //    /* Swap the values so that we always use ++ operator when iterating over the objects */
        //    Swap(columns_start, columns_end);

        //    moving_object_new_x = moving_object->GetXPosition() - move_offset;
        //    moving_object_new_y = moving_object->GetYPosition();
        //    break;
        //}
        //case 3 /* Right */:
        //{
        //    if (object_row_center < 0 || object_row_center >= world_.size()) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Right] - Row center is out of baoundaries");
        //        return 0.0f;
        //    }

        //    /*
        //        Going right in the world means going to bigger x word coordinates
        //        which means going to bigger number of column
        //    */
        //    if (object_column_center >= world_[0].size()) {
        //        dt::Console(dt::CRITICAL, "WorldSector::CheckCollision(): [Right] - Column center is bigger than the world columns");
        //        return 0.0f;
        //    }
        //    columns_start = (object_column_center < 0) ? 0 : object_column_center;
        //    columns_end = (columns_start + 1 >= world_[0].size()) ? world_[0].size() - 1 : columns_start + 1;
        //    rows_start = (object_row_center - 1 < 0) ? 0 : (object_row_center - 1);
        //    rows_end = (object_row_center + 1 >= world_.size()) ? world_.size() - 1 : (object_row_center + 1);

        //    moving_object_new_x = moving_object->GetXPosition() + move_offset;
        //    moving_object_new_y = moving_object->GetYPosition();
        //    break;
        //}
        //default:
        //    dt::Console(dt::WARNING, "WorldSector::CheckCollision(): Unrecognised direction");
        //    return 0.0f;
        //}

        //for (size_t i = rows_start; i <= rows_end; i++){
        //    for (size_t j = columns_start; j <= columns_end; j++){
        //        for (std::deque<WorldObject *>::iterator itr = world_[i][j].begin();
        //            itr != world_[i][j].end();
        //            ++itr)
        //        {
        //            WorldObject * neighbour = (*itr);
        //            CollisionType neighbour_collision_type = neighbour->GetCollisionType();
        //            if (neighbour_collision_type != CollisionType::COLLISION_NONE) {

        //                CollisionType moving_object_collision_type = moving_object->GetCollisionType();
        //                if (moving_object_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
        //                    
        //                    Rectangle2D_t mo_br(moving_object_new_x, moving_object_new_y, moving_object->GetObjectWidth(), moving_object->GetObjectHeight());
        //                    if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
        //                        Circle2D_t n_bc(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth() / 2.0);
        //                        CollisionCheck(mo_br, n_bc);
        //                    } else if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
        //                        Rectangle2D_t n_br(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth(), neighbour->GetObjectHeight());

        //                        if (CollisionCheck(mo_br, n_br)) return 0.0f;
        //                        
        //                    }
        //                } else if (moving_object_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
        //                    Circle2D_t mo_bc(moving_object_new_x, moving_object_new_y, moving_object->GetObjectWidth() / 2.0);
        //                    
        //                    if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_CIRCLE) {
        //                        Circle2D_t n_bc(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth() / 2.0);
        //                        CollisionCheck(mo_bc, n_bc);
        //                    } else if (neighbour_collision_type == CollisionType::COLLISION_BOUNDING_RECTANGLE) {
        //                        Rectangle2D_t n_br(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour->GetObjectWidth(), neighbour->GetObjectHeight());
        //                        CollisionCheck(n_br, mo_bc);
        //                    }
        //                }


        //                /* Get the type of collision of the moving object */
        //                /*CollisionConfig_t moving_object_collision_config = moving_object->GetCollision();
        //                CollisionConfig_t neighbour_collision_config = neighbour->GetCollision();
        //                
        //                if (CollisionCheck2DRectangles(rect_a, rect_neighbor)) {
        //                    if (direction == 0 || direction == 1)
        //                        return std::abs(std::abs(moving_object->GetYPosition() - neighbour->GetYPosition()) - moving_object_collision_config.parameter_ / 2.0f - neighbour_collision_config.parameter_ / 2.0f);

        //                    if (direction == 2 || direction == 3)
        //                        return std::abs(std::abs(moving_object->GetXPosition() - neighbour->GetXPosition()) - moving_object_collision_config.parameter_ / 2.0f - neighbour_collision_config.parameter_ / 2.0f);
        //                }
        //            } else if (neighbour_collision_config.type_ == CollisionType::COLLISION_BOUNDING_SPHERE) {
        //                Rectangle2D_t rect(moving_object_new_x, moving_object_new_y, moving_object_collision_config.parameter_, moving_object_collision_config.parameter_);
        //                Circle2D_t circ(neighbour->GetXPosition(), neighbour->GetYPosition(), neighbour_collision_config.parameter_);
        //                
        //                if (CollisionCheck2DCircleRectangle(circ, rect)) return 0.0f;*/

        //                
        //            }
        //        }
        //    }
        //}
        
        return collision_res;
    }

    size_t WorldSector::GetRow(float x) {
        return 0.0 + (world_[0].size()-1 - 0.0) * (x - x_margin_start_) / (x_margin_end_ - x_margin_start_);
    }

    size_t WorldSector::GetColumn(float y) {
        return 0.0 + (world_.size() - 1 - 0.0) * (y - y_margin_start_) / (y_margin_end_ - y_margin_start_);
    }

}

