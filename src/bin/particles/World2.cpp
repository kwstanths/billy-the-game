#include "World2.hpp"

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

World2::World2() : WorldSector() {
    is_inited_ = false;
}

int World2::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
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

    float hd = 0.3f;
    float vd = 0.3f;
    float dd = std::sqrt(hd*hd + vd * vd);
    size_t rows = 9;
    size_t cols = 7;
    std::vector<std::vector<Particle *>> particles(rows, std::vector<Particle *>(cols));
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            Particle * p = NewObj<Particle>();
            p->Init(Particle::SpawningMethod::RAND_VELOCITY, glm::vec3(-1.2f + hd * i, 7.5, -1.0f + vd * j), &rng_, &planes_, &triangles_, &spheres_, this, engine);
            p->SetForce(glm::vec3(0, -3, 0));
            p->SetVelocity(glm::vec3(0, 0, 0));
            p->SetSpringElasticity(4);
            p->SetSpringDamping(5);
            p->draw_spring_lines = false;
            p->draw_particle = true;
            particles[i][j] = p;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            /* streach spring forces */
            if (j - 1 >= 0) particles[i][j]->AddSpringForce(particles[i][j - 1], vd, 30);
            if (j + 1 < cols) particles[i][j]->AddSpringForce(particles[i][j + 1], vd, 30);
            if (i - 1 >= 0) particles[i][j]->AddSpringForce(particles[i - 1][j], hd, 30);
            if (i + 1 < rows) particles[i][j]->AddSpringForce(particles[i + 1][j], hd, 30);

            /* shear spring forces */
            if ((i - 1 >= 0) && (j - 1 >= 0)) particles[i][j]->AddSpringForce(particles[i - 1][j - 1], dd, 30);
            if ((i + 1 < rows) && (j - 1 >= 0)) particles[i][j]->AddSpringForce(particles[i + 1][j - 1], dd, 30);
            if ((i - 1 >= 0) && (j + 1 < cols)) particles[i][j]->AddSpringForce(particles[i - 1][j + 1], dd, 30);
            if ((i + 1 < rows) && (j + 1 < cols)) particles[i][j]->AddSpringForce(particles[i + 1][j + 1], dd, 30);

            /* bend spring forces */
            if (j - 2 >= 0) particles[i][j]->AddSpringForce(particles[i][j - 2], 2 * vd, 30);
            if (j + 2 < cols) particles[i][j]->AddSpringForce(particles[i][j + 2], 2 * vd, 30);
            if (i - 2 >= 0) particles[i][j]->AddSpringForce(particles[i - 2][j], 2 * hd, 30);
            if (i + 2 < rows) particles[i][j]->AddSpringForce(particles[i + 2][j], 2 * hd, 30);
        }
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
    sphere->spring_elasticity_k_ = 4;
    sphere->spring_damping_k_ = 5;
    sphere->draw_spring_triangles_ = true;

    for (size_t i = 0; i < rows - 1; i++) {
        for (size_t j = 0; j < cols - 1; j++) {
            sphere->cloth_triangles_.push_back(particles[i][j]);
            sphere->cloth_triangles_.push_back(particles[i][j + 1]);
            sphere->cloth_triangles_.push_back(particles[i + 1][j + 1]);

            sphere->cloth_triangles_.push_back(particles[i][j]);
            sphere->cloth_triangles_.push_back(particles[i + 1][j + 1]);
            sphere->cloth_triangles_.push_back(particles[i + 1][j]);
        }
    }

    is_inited_ = true;
    return 0;
}

int World2::Destroy() {

    is_inited_ = false;
    return true;
}
