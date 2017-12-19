#ifndef __GameWorld_hpp__
#define __GameWorld_hpp__

#include <vector>
#include <deque>

#include "WorldObject.hpp"

namespace game_engine {
    \
    class GameSector {
    public:
        GameSector();

        int Init(size_t width, size_t height, 
            float x_margin_start, float x_margin_end,
            float y_margin_start, float y_margin_end);

        int Destroy();

        WorldObject * NewObj(float x, float y);

    private:
        
        bool is_inited_;
        float x_margin_start_, x_margin_end_, y_margin_start_, y_margin_end_;
        
        std::vector<std::vector<std::deque<WorldObject *> > >world_;
        std::vector<WorldObject> objects_;

        /**
        
        */    
        size_t GetXPosition(float x);
        
        /**
        
        */
        size_t GetYPosition(float y);

        /**
        
        */
        size_t GetSectorID(size_t index_x, size_t index_y);
    };

}

#endif