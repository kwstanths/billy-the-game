#ifndef __WorldPortal_hpp__
#define __WorldPortal_hpp__

#include "game_engine/InteractableObject.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/GameEngine.hpp"

#include "Camera.hpp"

class WorldPortal : game_engine::Interactablebject {
public:

    bool Init(game_engine::math::Vector3D pos, game_engine::WorldSector * world, 
        game_engine::WorldSector * other_world, game_engine::math::Vector3D other_pos, game_engine::GameEngine * engine, Camera * camera);

    virtual void Interact() override;

private:
    game_engine::GameEngine * engine_;

    game_engine::WorldSector * other_world_;
    game_engine::math::Vector3D position_;
};

#endif