#include "GameWorld.hpp"

namespace game_engine {

    GameWorld::GameWorld() {
        is_inited_ = false;
    }

    int GameWorld::Init(size_t rows, size_t columns) {
        world_ = std::vector<std::vector<WorldObject>>(rows, std::vector<WorldObject>(columns));

        is_inited_ = true;
        return 0;
    }

    int GameWorld::Destroy() {
        world_.clear();

        is_inited_ = false;
        return 0;
    }

    WorldObject * GameWorld::GetNewObject(float x, float y) {
        return &world_[x][y];
    }

}

