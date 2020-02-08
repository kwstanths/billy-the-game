#include "World.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/RNG.hpp"
#include "debug_tools/Console.hpp"

#include "Particle.hpp"
#include "Wall.hpp"
#include "Fire.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Water.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(20, 20, -15.0f, 15.0f, -15.0f, 15.0f, 10 * 10);
    if (ret) return ret;

    planes_.push_back(Plane(glm::vec3(0), glm::vec3(0, 1, 0)));
    planes_.push_back(Plane(glm::vec3(5, 0, 0), glm::vec3(-1, 0, 0)));
    planes_.push_back(Plane(glm::vec3(-5, 0, 0), glm::vec3(1, 0, 0)));
    planes_.push_back(Plane(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1)));
    planes_.push_back(Plane(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1)));

    rng_ = math::MersenneTwisterGenerator(time(NULL));

    /* Initialize particles */
    std::vector<Particle *> particles;
    for (size_t i = 0; i < 16; i++) {
        Particle * p = NewObj<Particle>();
        p->Init(Particle::SpawningMethod::Fountain, glm::vec3(0, 5, 0), &rng_, &planes_, &triangles_, &spheres_, this, engine);
        p->SetForce(glm::vec3(0, -3, 0 ));
        p->SetFriction(0.1f);
        p->SetLifetime(2.0f + rng_.genrand_real3() * 5.0f);
        p->draw_spring_lines = false;
        p->draw_particle = true;
        particles.push_back(p);
    }

    ///* Set spring forces */
    //particles[0]->SetFixed(true);
    //particles[0]->AddSpringForce(particles[1], 0.2f, 30);
    //for (size_t i = 1; i < particles.size() - 1; i++) {
    //    particles[i]->AddSpringForce(particles[i - 1], 0.2f, 30);
    //    particles[i]->AddSpringForce(particles[i + 1], 0.2f, 30);
    //}
    //particles[particles.size() - 1]->AddSpringForce(particles[particles.size() - 2], 0.2f, 30);

    Water * water = NewObj<Water>();
    water->Init(0.0f, 1.1f, 0.0f, this);

    /* Create graphics objects */
    /* Create floor */
    Wall * wall = NewObj<Wall>();
    wall->Init(0.0f, 0.0f, 0.0f, this);
    wall->Scale(10, 10, 10);
    wall->Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));

    wall = NewObj<Wall>();
    wall->Init(5.0f, 2.5f, 0.0f, this);
    wall->Scale(10, 5, 5);
    wall->Rotate(ge::math::GetRadians(270.0f), glm::vec3(0, 1, 0));

    wall = NewObj<Wall>();
    wall->Init(-5.0f, 2.5f, 0.0f, this);
    wall->Scale(10, 5, 5);
    wall->Rotate(ge::math::GetRadians(90.0f), glm::vec3(0, 1, 0));

    wall = NewObj<Wall>();
    wall->Init(0.0f, 2.5f, 5.0f, this);
    wall->Scale(10, 5, 5);
    wall->Rotate(ge::math::GetRadians(180.0f), glm::vec3(0, 1, 0));

    wall = NewObj<Wall>();
    wall->Init(0.0f, 2.5f, -5.0f, this);
    wall->Scale(10, 5, 5);

    Fire * fire = NewObj<Fire>();
    fire->Init(0.0f, 10.0f, 0.0f, this, engine);

    /*TriangleObject * triangle = NewObj<TriangleObject>();
    triangle->Init(0, 0, 0, this, glm::vec3(0, 3, -4), glm::vec3(-4, 3, -0), glm::vec3(-4, 5, -4));

    triangle = NewObj<TriangleObject>();
    triangle->Init(0, 0, 0, this, glm::vec3(4, 3, 0), glm::vec3(4, 5, 4), glm::vec3(0, 3, 4));

    SphereObject * sphere = NewObj< SphereObject>();
    sphere->Init(0, 2, 0, 2, this);
    sphere->method_ = Particle::IntegrationMethod::Verlet;
    sphere->bouncing_ = 0.3;
    sphere->friction_ = 0.1;
    sphere->spring_elasticity_k_ = 30;
    sphere->spring_damping_k_ = 0.2;
    sphere->draw_spring_triangles_ = 0;*/

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}
