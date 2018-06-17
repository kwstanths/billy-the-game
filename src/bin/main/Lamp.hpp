#ifndef __Lamp_hpp__
#define __Lamp_hpp__

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/Renderer.hpp"


class Lamp : public game_engine::WorldObject {
public:

    bool Init(float x, float y, float z, game_engine::GameEngine * engine);

    virtual void Draw(game_engine::Renderer * renderer) override;

private:
    glm::vec3 light_intensity_;
};

#endif