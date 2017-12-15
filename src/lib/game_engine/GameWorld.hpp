#ifndef __GameWorld_hpp__
#define __GameWorld_hpp__

#include <vector>

#include "WorldObject.hpp"

namespace game_engine {

    class GameWorld {
    public:
        GameWorld();

        int Init(size_t rows, size_t columns);

        int Destroy();

        WorldObject * GetNewObject(float x, float y);

    private:
        bool is_inited_;
        std::vector<std::vector<WorldObject> > world_;
    };

}

#endif