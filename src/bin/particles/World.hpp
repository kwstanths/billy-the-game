#ifndef __World_hpp__
#define __World_hpp__

#include "game_engine/WorldSector.hpp"
#include "game_engine/GameEngine.hpp"
#include "game_engine/math/RNG.hpp"


#include "Input.hpp"
#include "Camera.hpp"
#include "CAGeometry.hpp"

class World :public game_engine::WorldSector {
public:
    World();

    int Init(Input * input, Camera * camera, game_engine::GameEngine * engine);

    int Destroy();


private:
    std::vector<Plane> planes_;
    std::vector<Triangle> triangles_;
    std::vector<Sphere> spheres_;
    game_engine::math::MersenneTwisterGenerator rng_;
    bool is_inited_;

};

#endif