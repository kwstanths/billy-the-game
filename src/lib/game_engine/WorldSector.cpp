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
            dt::Console(dt::CRITICAL, "Game sector is not initialised");
            return nullptr;
        }
        if (sector_id_ >= objects_.size()) {
            dt::Console(dt::CRITICAL, "Game sector's objects size has reached its limit");
            return nullptr;
        }

        objects_[sector_id_].SetPosition(x, y, z);
        WorldObject * the_new_object = &objects_[sector_id_];
        sector_id_++;

        size_t index_row = GetYPosition(y);
        size_t index_col = GetXPosition(x);

        world_[index_row][index_col].push_back(the_new_object);

        return the_new_object;
    }

    size_t WorldSector::GetObjectsWindow(float center_x, float center_y, float margin, 
        std::vector<WorldObject*> & visible_world) 
    {
        size_t index = 0;
        size_t center_row = GetYPosition(center_y);
        size_t center_col = GetXPosition(center_x);
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

    bool WorldSector::CheckCollision(float move_offset, size_t direction) {

        return false;
    }

    size_t WorldSector::GetXPosition(float x) {
        return 0.0 + (world_[0].size()-1 - 0.0) * (x - x_margin_start_) / (x_margin_end_ - x_margin_start_);
    }

    size_t WorldSector::GetYPosition(float y) {
        return 0.0 + (world_.size() - 1 - 0.0) * (y - y_margin_start_) / (y_margin_end_ - y_margin_start_);
    }

}

