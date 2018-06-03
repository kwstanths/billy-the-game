#include "WorldSector.hpp"
#include "GameEngine.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "ErrorCodes.hpp"
#include "Collision.hpp"
#include "physics/HelpFunctions.hpp"
#include "physics/Types.hpp"

namespace dt = debug_tools;
namespace ms = game_engine::memory_subsystem;
namespace ph = game_engine::physics;

namespace game_engine {

    WorldSector::WorldSector() {
        physics_engine_ = new ph::PhysicsEngine();
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

        physics_engine_->Init(Rectangle2D(0, 0, 250, 250), elements);

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

    void WorldSector::UpdateObjectPosition(WorldObject * object, float old_pos_x, float old_pos_y, float new_pos_x, float new_pos_y) {
        
        /* TODO Do some checkig on th object */
        
        /* Find the old and new position */
        size_t old_pos_index_row = GetColumn(old_pos_y);
        size_t old_pos_index_col = GetRow(old_pos_x);
        size_t new_pos_index_row = GetColumn(new_pos_y);
        size_t new_pos_index_col = GetRow(new_pos_x);
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
        size_t col_end = (center_col + border < world_[0].size()) ? (center_col + border) : (world_[0].size() -1);

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

    size_t WorldSector::GetRow(float x) {
        float index = 0.0f + (world_[0].size() - 1 - 0.0f) * (x - x_margin_start_) / (x_margin_end_ - x_margin_start_);
        return static_cast<size_t>(index);
    }

    size_t WorldSector::GetColumn(float y) {
        float index = 0.0f + (world_.size() - 1 - 0.0f) * (y - y_margin_start_) / (y_margin_end_ - y_margin_start_);
        return static_cast<size_t>(index);
    }

}

