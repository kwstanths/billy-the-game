#ifndef __GameWorld_hpp__
#define __GameWorld_hpp__

#include <vector>
#include <deque>

#include "WorldObject.hpp"
#include "physics/Types.hpp"

namespace game_engine {
    
    /**
        A class to store world objects in a 2d manner
    */
    class WorldSector {
    public:
        /**
            Does nothing in particular
        */
        WorldSector();

        /**
            Initializes the internal structures used to hold the world objects
            @param width The number of columns to be used in the 2d world
        */
        int Init(size_t width, size_t height, 
            float x_margin_start, float x_margin_end,
            float y_margin_start, float y_margin_end, 
            size_t elements);

        int Destroy();

        /**
            Add a new object to the sextor
            @param x Position x
            @param y Position y
            @param z Position z
            @return A pointer to the world object
        */
        WorldObject * NewObj(float x, float y, float z);

        /**
            Get a window of object in the world. Objects are assigned sequentially to the visible world 
            vector. Does not allocate any new memory
            @param center_x The x coordinate of the window's center
            @param center_y The y coordinate of the window's center
            @param margin The top, down, left, right window margin
            @param[out] visible_world The vector with pointers to the objects window
            @return The number of objects assigned to the visible_world vector
        */
        size_t GetObjectsWindow(float center_x, float center_y, float margin, std::vector<WorldObject *> & visible_world);

        /**
            Check for collision inside the world sector, provided the direction of the moving object
            @param moving_object The object to check for collision with the world. Should have SetCollision() used
            @param move_offset The amount of moving required
            @param direction The direction of the movement, 0=Up, 1=Bottom, 2=Left, 3=Right 
        */
        CollisionResult_t CheckCollision(WorldObject * moving_object, float move_offset, Direction direction);

    private:
        
        bool is_inited_;
        size_t sector_id_;
        float x_margin_start_, x_margin_end_, y_margin_start_, y_margin_end_;
        
        /* 
            A struct that resembles the world. Holds pointers to actual objects that are stored 
            sequentially
        */
        std::vector<std::vector<std::deque<WorldObject *> > >world_;
        std::vector<WorldObject> objects_;

        /**
        
        */    
        size_t GetRow(float x);
        
        /**
        
        */
        size_t GetColumn(float y);

        /**
            Get the vertical and horizontal distance from the first colliding object in the world, if the
            moving object is moved to the new_position
            @param moving_object The object to be moved
            @param new_position The new center position
            @return <vertical, horizontal>
        */
        std::pair<float, float> CollisionGetDistance(WorldObject * moving_object, Point2D_t new_position);
    };

}

#endif