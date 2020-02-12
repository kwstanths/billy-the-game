#include "World0.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/RNG.hpp"
#include "debug_tools/Console.hpp"

#include "Particle.hpp"
#include "Wall.hpp"
#include "Fire.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World0::World0() : WorldSector() {
    is_inited_ = false;
}

int World0::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(20, 20, -15.0f, 15.0f, -15.0f, 15.0f, 10 * 10);
    if (ret) return ret;

    planes_.push_back(Plane(glm::vec3(0), glm::vec3(0, 1, 0)));
    planes_.push_back(Plane(glm::vec3(10, 0, 0), glm::vec3(-1, 0, 0)));
    planes_.push_back(Plane(glm::vec3(-10, 0, 0), glm::vec3(1, 0, 0)));
    planes_.push_back(Plane(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1)));
    planes_.push_back(Plane(glm::vec3(0, 0, -10), glm::vec3(0, 0, 1)));

    triangles_.push_back(Triangle(glm::vec3(0, 3, -4), glm::vec3(-4, 3, -0), glm::vec3(-4, 5, -4)));
    triangles_.push_back(Triangle(glm::vec3(4, 3, 0), glm::vec3(4, 5, 4), glm::vec3(0, 3, 4)));

    spheres_.push_back(Sphere(glm::vec3(0, 2, 0), 2));

    rng_ = math::MersenneTwisterGenerator(time(NULL));

    size_t number_of_particles = 30;
    float spacing = 0.17f;
    std::vector<Particle *> particles(number_of_particles);
    for (size_t i = 0; i < number_of_particles; i++) {
        Particle * p = NewObj<Particle>();
        p->Init(Particle::SpawningMethod::NONE, glm::vec3(-2.5f + spacing * i, 6, -2.5f + spacing * i), &rng_, &planes_, &triangles_, &spheres_, this, engine);
        p->SetForce(glm::vec3(0, -3, 0));
        p->SetVelocity(glm::vec3(0, 0, 0));
        p->SetSpringElasticity(15);
        p->SetSpringDamping(1);
        p->draw_particle = true;
        p->draw_spring_lines = true;
        particles[i] = p;
    }

    particles[0]->SetFixed(true);
    for (int i = 0; i < number_of_particles - 1; i++) {
        particles[i]->AddSpringForce(particles[i + 1], std::sqrt(2 * spacing * spacing), 50);
    }
    for (int i = 1; i < number_of_particles ; i++) {
        particles[i]->AddSpringForce(particles[i - 1], std::sqrt(2 * spacing * spacing), 50);
    }

    /* Create floor */
    Wall * wall = NewObj<Wall>();
    wall->Init(0.0f, 0.0f, 0.0f, this);
    wall->Scale(20, 20, 20);
    wall->Rotate(ge::math::GetRadians(90.0f), glm::vec3(-1, 0, 0));

    wall = NewObj<Wall>();
    wall->Init(10.0f, 5.0f, 0.0f, this);
    wall->Scale(20, 10, 10);
    wall->Rotate(ge::math::GetRadians(270.0f), glm::vec3(0, 1, 0));

    wall = NewObj<Wall>();
    wall->Init(-10.0f, 5.0f, 0.0f, this);
    wall->Scale(20, 10, 10);
    wall->Rotate(ge::math::GetRadians(90.0f), glm::vec3(0, 1, 0));

    wall = NewObj<Wall>();
    wall->Init(0.0f, 5.0f, 10.0f, this);
    wall->Scale(20, 10, 10);
    wall->Rotate(ge::math::GetRadians(180.0f), glm::vec3(0, 1, 0));

    wall = NewObj<Wall>();
    wall->Init(0.0f, 5.0f, -10.0f, this);
    wall->Scale(20, 10, 10);

    Fire * fire = NewObj<Fire>();
    fire->Init(0.0f, 10.0f, 0.0f, this, engine);

    TriangleObject * triangle = NewObj<TriangleObject>();
    triangle->Init(0, 0, 0, this, glm::vec3(0, 3, -4), glm::vec3(-4, 3, -0), glm::vec3(-4, 5, -4));

    triangle = NewObj<TriangleObject>();
    triangle->Init(0, 0, 0, this, glm::vec3(4, 3, 0), glm::vec3(4, 5, 4), glm::vec3(0, 3, 4));

    SphereObject * sphere = NewObj< SphereObject>();
    sphere->Init(0, 2, 0, 2, this);
    sphere->method_ = Particle::IntegrationMethod::Verlet;
    sphere->bouncing_ = 0.3;
    sphere->friction_ = 0.0;
    sphere->spring_elasticity_k_ = 15;
    sphere->spring_damping_k_ = 1;
    sphere->draw_spring_triangles_ = false;

    is_inited_ = true;
    return 0;
}

int World0::Destroy() {

    is_inited_ = false;
    return true;
}
