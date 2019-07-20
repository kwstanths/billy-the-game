#ifndef __PhysicsObject_hpp__
#define __PhysicsObject_hpp__

#include "game_engine/math/Types.hpp"

namespace game_engine {
namespace physics {

    /**
        A physics object
    */
    class PhysicsObject {
    public:
        /**
            Does nothing in particular. Call Init()
        */
        PhysicsObject();

        virtual ~PhysicsObject() {};

        /**
            Initializes the physics object
            @param pos_x The position x
            @param pos_y The position y
            @param pos_z The position z
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(game_engine::Real_t pos_x, game_engine::Real_t pos_y, game_engine::Real_t pos_z);

        /**
            Removes from the pgysics engine, TODO
            @return 0=OK
        */
        int Destroy();

        /**
            Get whether the object is initialised or not
            @return true = initialised, false = not initialised
        */
        bool IsInited();

        /**
            Get the position x coordinate
            @return position x coordinate
        */
        game_engine::Real_t GetX();

        /**
            Get the position y coordinate
            @return position y coordinate
        */
        game_engine::Real_t GetY();

        /**
            Get the position z coordinate
            @return position z coordinate
        */
        game_engine::Real_t GetZ();

        /**
            Set the position of the object. Does not update the object's position in the physics engine
            @param pos_x Position x coordinate
            @param pos_y Position y coordinate
            @param pos_z Position z coordinate
        */
        void SetPosition(game_engine::Real_t pos_x, game_engine::Real_t pos_y, game_engine::Real_t pos_z);

    protected:
        bool removable_;

    private:
        bool is_inited_;
        game_engine::Real_t pos_x_, pos_y_, pos_z_;
        size_t object_type_ = 0;

    };

}

}

#endif