#include "WorldSector.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"
namespace ph = game_engine::physics;

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"
namespace dt = debug_tools;

#include "ErrorCodes.hpp"
namespace ms = game_engine::memory_subsystem;

namespace game_engine {

    WorldSector::WorldSector() {
        physics_engine_ = new ph::PhysicsEngine();
        is_inited_ = false;
    }

    int WorldSector::Init(size_t width, size_t height, 
        float x_margin_start, float x_margin_end,
        float y_margin_start, float y_margin_end,
        size_t elements) 
    {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* Initialize the world structure */
        world_ = std::vector<std::vector<std::deque<WorldObject *> > >(height, std::vector<std::deque<WorldObject *> >(width));
        array_allocator_ = new ms::ArrayAllocator();
        array_allocator_->Init(500 * 500);
        pool_allocator_ = new ms::PoolAllocator();
        pool_allocator_->Init(sizeof(WorldObject), 1000);

        /* Initialize the physics engine used */
        physics_engine_->Init(Rectangle2D(0, 0, 250, 250), elements);

        /* Set the margins */
        x_margin_start_ = x_margin_start;
        x_margin_end_ = x_margin_end;
        y_margin_start_ = y_margin_start;
        y_margin_end_ = y_margin_end;

        is_inited_ = true;
        return 0;
    }

    int WorldSector::Destroy() {
        
        CodeReminder("Iterate through the world objects, and call their Destroy()");
        
        world_.clear();
        array_allocator_->Destroy();
        pool_allocator_->Destroy();
         
        is_inited_ = false;
        return 0;
    }

    bool WorldSector::IsInited() {
        return is_inited_;
    }

    int WorldSector::Insert(WorldObject * object, float x, float y, float z) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        size_t index_row = GetRow(y);
        size_t index_col = GetColumn(x);
        world_[index_row][index_col].push_back(object);
        /* This is probably obsolete */
        object->world_sector_ = this;
        return 0;
    }

    void WorldSector::UpdateObjectPosition(WorldObject * object, float old_pos_x, float old_pos_y, float new_pos_x, float new_pos_y) {
        
        /* TODO Do some checkig on th object */
        
        /* Find the old and new position */
        size_t old_pos_index_row = GetRow(old_pos_y);
        size_t old_pos_index_col = GetColumn(old_pos_x);
        size_t new_pos_index_row = GetRow(new_pos_y);
        size_t new_pos_index_col = GetColumn(new_pos_x);
        /* If no moving is required then leave */
        
        if ((old_pos_index_row == new_pos_index_row) && (old_pos_index_col == new_pos_index_col)) return;

        /* Remove from the old position */
        std::deque<WorldObject *> & objects = world_[old_pos_index_row][old_pos_index_col];
        for (std::deque<WorldObject *>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
            if (*itr == object) {
                objects.erase(itr);
                break;
            }
        }
        
        /* Add to new position */
        world_[new_pos_index_row][new_pos_index_col].push_back(object);
    }

    void WorldSector::Remove(WorldObject * object) {
        size_t index_row = GetRow(object->GetY());
        size_t index_col = GetColumn(object->GetX());

        std::deque<WorldObject *> & objects = world_[index_row][index_col];
        for (std::deque<WorldObject *>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
            if (*itr == object) {
                objects.erase(itr);
                break;
            }
        }

        if (object->removable_) delete_vector_.push_back(object);
        else dt::Console(dt::WARNING, "Memory leak: Removing object that was not made removable");
    }

    size_t WorldSector::GetObjectsWindow(float center_x, float center_y, float margin,
        std::vector<WorldObject*> & objects) 
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
        size_t col_end = (center_col + border < world_[0].size()) ? (center_col + border) : (world_[0].size() -1);

        for (size_t i = rows_start; i <= rows_end; i++) {
            for (size_t j = col_start - border; j <= col_end; j++) {
                for (std::deque<WorldObject*>::iterator itr = world_[i][j].begin(); itr != world_[i][j].end(); ++itr) {
                    if (index > objects.size() - 1) {
                        dt::Console(dt::WARNING, "WorldSector::GetObjetcsWindow() Objects overflow");
                        return index;
                    }
                    objects[index++] = *itr;
                }

            }
        }
        return index;
    }

    WorldObject * WorldSector::FindInteractNeighbour(Rectangle2D search_area, float pos_x, float pos_y) {
        std::vector<ph::PhysicsObject *> area_objects(10, nullptr);
        size_t nof = physics_engine_->GetObjectsArea(search_area, area_objects);

        //ph::PhysicsObject * neighbour = area_objects[0];
        ph::PhysicsObject * neighbour = nullptr;
        /* Find the first interactable object in the area */
        for (size_t i = 0; i < nof; i++) {
            WorldObject * temp = dynamic_cast<WorldObject *>(area_objects[i]);
            if (temp->interactable_) return temp;
        }

        return nullptr;
    }

    ph::PhysicsEngine * WorldSector::GetPhysicsEngine() {

        return physics_engine_;
    }

    size_t WorldSector::GetRow(float vertical_coordinate) {
        float index = 0.0f + (world_.size() - 1 - 0.0f) * (vertical_coordinate - y_margin_start_) / (y_margin_end_ - y_margin_start_);
        return static_cast<size_t>(index);
    }

    size_t WorldSector::GetColumn(float horizontal_coordinate) {
        float index = 0.0f + (world_[0].size() - 1 - 0.0f) * (horizontal_coordinate - x_margin_start_) / (x_margin_end_ - x_margin_start_);
        return static_cast<size_t>(index);
    }

    void WorldSector::DeleteRemovedObjects() {
        for (std::deque<WorldObject *>::iterator itr = delete_vector_.begin(); itr != delete_vector_.end(); ++itr) {
            pool_allocator_->Deallocate(*itr);
        }
        delete_vector_.clear();
    }

}

