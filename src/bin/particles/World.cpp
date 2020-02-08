#include "World.hpp"

#include "debug_tools/Console.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/RNG.hpp"
#include "game_engine/utility/BasicFunctions.hpp"

#include "Particle.hpp"
#include "Wall.hpp"
#include "Fire.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Floor.hpp"

#include "cal3d/coremodel.h"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(40, 40, 0.0f, 41.0f, 0.0f, 41.0f, 40 * 40);
    if (ret) return ret;

    rng_ = math::MersenneTwisterGenerator(time(NULL));
    
    ReadMap("map.csv");

    for (size_t i = 0; i < 15; i++) {
        std::pair<int, int> random_pos = empty_positions_[rng_.genrand_real3() * empty_positions_.size()];
        
        /* Initialize particles */
        Particle * p = NewObj<Particle>();
        p->Init(&empty_positions_, &rng_, this, grid_, engine);
        p->SetForce(glm::vec3(0, 0, 0));
        p->SetFriction(0.0f);
        p->SetBouncing(1.0f);
    }

    /* Initialize particles */
    /*Particle * p = NewObj<Particle>();
    p->Init(&empty_positions_, &rng_, this, grid_, engine);
    p->SetForce(glm::vec3(0, 0, 0));
    p->SetFriction(0.0f);
    p->SetBouncing(0.2f);
    p->Set1();

    p = NewObj<Particle>();
    p->Init(&empty_positions_, &rng_, this, grid_, engine);
    p->SetForce(glm::vec3(0, 0, 0));
    p->SetFriction(0.0f);
    p->SetBouncing(0.2f);
    p->Set2();*/

    /* Build the walls */
    NewObj<Wall>()->Init(0, 0, 0, this);
    /* Build the floor */
    NewObj<Floor>()->Init(MAP_X / 2, 0, MAP_Y / 2, this);

    Fire * fire = NewObj<Fire>();
    fire->Init(0.0f, 10.0f, 0.0f, this, engine);

    SphereObject * sphere = NewObj< SphereObject>();
    sphere->Init(0, 2, 0, 2, this);
    sphere->method_ = Particle::IntegrationMethod::EulerSemi;
    sphere->bouncing_ = 0.1;
    sphere->friction_ = 0.1;
    sphere->spring_elasticity_k_ = 30;
    sphere->spring_damping_k_ = 0.2;
    sphere->draw_spring_triangles_ = 0;

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}

void World::ReadMap(std::string name) {

    std::vector<std::vector<std::string>> map_;

    std::string line;
    std::ifstream myfile(name);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            map_.push_back(ge::utility::split(line, ","));
        }
        myfile.close();
    }
    else {
        dt::Console(dt::FATAL, "Could not open map.csv...");
        return;
    }

    grid_ = new ge::utility::UniformGrid<int, 2>({ MAP_X, MAP_Y});

    for (int i = 0; i < MAP_X; i++) {
        for (int j = 0; j < MAP_Y; j++) {
            if (map_[i][j] == "7") {
                grid_->at(i, j) = -1;
            } else {
                grid_->at(i, j) = 0;
                empty_positions_.push_back(std::pair(i, j));
            }
        }
    }

}
