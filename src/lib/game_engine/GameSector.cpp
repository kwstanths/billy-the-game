#include "GameSector.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameSector::GameSector() {
        is_inited_ = false;
    }

    int GameSector::Init(size_t width, size_t height, 
        float x_margin_start, float x_margin_end,
        float y_margin_start, float y_margin_end) 
    {
        world_ = std::vector<std::vector<std::deque<WorldObject *> > >(height, std::vector<std::deque<WorldObject *> >(width));
        
        x_margin_start_ = x_margin_start;
        x_margin_end_ = x_margin_end;
        y_margin_start_ = y_margin_start;
        y_margin_end_ = y_margin_end;

        is_inited_ = true;
        return 0;
    }

    int GameSector::Destroy() {
        world_.clear();

        is_inited_ = false;
        return 0;
    }

    WorldObject * GameSector::NewObj(float x, float y) {
        if (!is_inited_) {
            dt::Console(dt::FATAL, "Game world not initialised");
            return nullptr;
        }

        objects_.push_back(WorldObject());
        WorldObject * the_new_object = &(objects_.at(objects_.size() - 1));
        the_new_object->SetPosition(x, y, 0.0f);

        size_t index_x = GetXPosition(x);
        size_t index_y = GetYPosition(y);
        world_[index_x][index_y].push_back(the_new_object);

        return the_new_object;
    }

    size_t GameSector::GetXPosition(float x) {
        return 0.0 + (world_[0].size()-1 - 0.0) * (x - x_margin_start_) / (x_margin_end_ - x_margin_start_);
    }

    size_t GameSector::GetYPosition(float y) {
        return 0.0 + (world_.size() - 1 - 0.0) * (y - y_margin_start_) / (y_margin_end_ - y_margin_start_);
    }

    size_t GameSector::GetSectorID(size_t index_x, size_t index_y) {
        return index_y * world_.size() + index_y;
    }

}

