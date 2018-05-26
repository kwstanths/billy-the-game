#ifndef __Treasure_hpp__
#define __Treasure_hpp__

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "game_engine/WorldObject.hpp"

class Treasure : public game_engine::WorldObject {
public:

    bool Init(game_engine::OpenGLObject * object, game_engine::OpenGLTexture * texture);

    virtual void Step(double delta_time) override;

private:

};

#endif