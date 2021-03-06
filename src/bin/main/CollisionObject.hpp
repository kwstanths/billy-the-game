#ifndef __CollisionObject_hpp__
#define __CollisionObject_hpp__

#include <string>

#include "game_engine/core/GameEngine.hpp"
#include "game_engine/core/WorldObject.hpp"
#include "game_engine/core/WorldSector.hpp"
#include "game_engine/math/Types.hpp"

#include "Maps/MapProperties.hpp"

/**
    A physics only object to represent a collidable object in the world 
*/
class CollisionObject : public game_engine::physics::PhysicsObject {
public:

    /* Most of these arguments are probably useless */
    bool Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, std::string collision_string, game_engine::WorldSector * world,
        game_engine::GameEngine * engine);

private:
};

#endif