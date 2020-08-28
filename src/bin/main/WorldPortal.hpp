#ifndef __WorldPortal_hpp__
#define __WorldPortal_hpp__

#include "game_engine/InteractableObject.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/GameEngine.hpp"

#include "Camera.hpp"
#include "Maps/World.hpp"

/* This object functions as a connection between two worlds */
class WorldPortal : game_engine::Interactablebject {
public:

    bool Init(game_engine::math::Vector3D pos, World * world, 
        World * other_world, game_engine::math::Vector3D other_pos, game_engine::GameEngine * engine, Camera * camera);

    virtual void Interact() override;

private:
    game_engine::GameEngine * engine_;

    World * this_world_;
    World * other_world_;
    /* The position in the other world, for the player and the camera */
    game_engine::math::Vector3D position_;
};

#endif