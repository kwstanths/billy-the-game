#ifndef __Triangle_hpp__
#define __Triangle_hpp__

#include "glm/glm.hpp"

#include "game_engine/GameEngine.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/Vector.hpp"

class TriangleObject : public game_engine::WorldObject {
public:
    TriangleObject();

    int Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::WorldSector * world,
        glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

    void Draw(game_engine::graphics::Renderer * renderer);

private:
    glm::vec3 v1_, v2_, v3_;
    bool is_inited_;
};


#endif