#ifndef __Collision_hpp__
#define __Collision_hpp__


namespace game_engine {

    /**
        Types of collision supported
    */
    enum CollisionType {
        COLLISION_NONE,
        COLLISION_BOUNDING_RECTANGLE,
        COLLISION_BOUNDING_SPHERE,
    };

    /**
        Collision configuration struct
    */
    typedef struct {
        /* The collision type */
        CollisionType type_;
        /*
            If type_ is NONE, parameter is ignored
            If type_ is BOUNDING_RECTANGLE, parameter is the side length
            If type_ is BOUNDING_SPHERE, parameter is the radius
        */
        size_t parameter_;
    } CollisionConfig_t;

    /**
        Result of collision detection
    */
    typedef struct {
        bool move_up = false;
        bool move_down = false;
        bool move_left = false;
        bool move_right = false;
    } CollisionResult_t;



}


#endif