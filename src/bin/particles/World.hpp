#ifndef __World_hpp__
#define __World_hpp__

#include "game_engine/WorldSector.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/GameEngine.hpp"
#include "game_engine/math/RNG.hpp"
#include "game_engine/utility/UniformGrid.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "CAGeometry.hpp"

class World :public game_engine::WorldSector {
    friend class Particle;
public:
    World();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();

    game_engine::utility::UniformGrid<int, 2> * grid_;
private:
    const size_t MAP_X = 40;
    const size_t MAP_Y = 40;
    std::vector<std::pair<int, int>> empty_positions_;

    std::vector<Plane> planes_;
    std::vector<Triangle> triangles_;
    std::vector<Sphere> spheres_;
    game_engine::math::MersenneTwisterGenerator rng_;
    bool is_inited_;

    void ReadMap(std::string name);
};

#endif