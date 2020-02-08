#include "World4.hpp"

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

World4::World4() : WorldSector() {
    is_inited_ = false;
}

int World4::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(20, 20, -15.0f, 15.0f, -15.0f, 15.0f, 10 * 10);
    if (ret) return ret;

    planes_.push_back(Plane(glm::vec3(0), glm::vec3(0, 1, 0)));
    planes_.push_back(Plane(glm::vec3(10, 0, 0), glm::vec3(-1, 0, 0)));
    planes_.push_back(Plane(glm::vec3(-10, 0, 0), glm::vec3(1, 0, 0)));
    planes_.push_back(Plane(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1)));
    planes_.push_back(Plane(glm::vec3(0, 0, -10), glm::vec3(0, 0, 1)));

    spheres_.push_back(Sphere(glm::vec3(0, 7, 0), 2));

    rng_ = math::MersenneTwisterGenerator(time(NULL));

    float xyzd = 0.2f;
    float dd = std::sqrt(xyzd*xyzd + xyzd * xyzd);
    float cdd = std::sqrt(xyzd*xyzd + dd * dd);
    size_t x_size = 4;
    size_t y_size = 4;
    size_t z_size = 4;
    std::vector<std::vector<std::vector<Particle *>>> particles(x_size, std::vector<std::vector<Particle *>>(y_size, std::vector<Particle *>(z_size)));
    for (size_t i = 0; i < x_size; i++) {
        for (size_t j = 0; j < y_size; j++) {
            for (size_t k = 0; k < z_size; k++) {
                Particle * p = NewObj<Particle>();
                p->Init(Particle::SpawningMethod::NONE, glm::vec3(-1.5f + xyzd * i, 10.0f + xyzd * j, -0.7f + xyzd * k), &rng_, &planes_, &triangles_, &spheres_, this, engine);
                p->SetForce(glm::vec3(0, -3, 0));
                p->SetVelocity(glm::vec3(0, 0, 0));
                p->SetIntegrationMethod(Particle::EulerOrig);
                p->SetBouncing(0.99);
                p->SetFriction(0.1);
                p->SetSpringElasticity(5);
                p->SetSpringDamping(1);
                p->draw_spring_lines = false;
                p->draw_particle = true;
                particles[i][j][k] = p;
            }
        }
    }

    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            for (int k = 0; k < z_size; k++) {

                if (i - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i - 1][j][k], xyzd, 30);
                if (i + 1 < x_size) particles[i][j][k]->AddSpringForce(particles[i + 1][j][k], xyzd, 30);
                if (i - 2 >= 0) particles[i][j][k]->AddSpringForce(particles[i - 2][j][k], 2*xyzd, 30);
                if (i + 2 < x_size) particles[i][j][k]->AddSpringForce(particles[i + 2][j][k], 2*xyzd, 30);

                if (j - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i][j - 1][k], xyzd, 30);
                if (j + 1 < y_size) particles[i][j][k]->AddSpringForce(particles[i][j + 1][k], xyzd, 30);
                if (j - 2 >= 0) particles[i][j][k]->AddSpringForce(particles[i][j - 2][k], 2*xyzd, 30);
                if (j + 2 < y_size) particles[i][j][k]->AddSpringForce(particles[i][j + 2][k], 2*xyzd, 30);

                if (k - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i][j][k - 1], xyzd, 30);
                if (k + 1 < z_size) particles[i][j][k]->AddSpringForce(particles[i][j][k + 1], xyzd, 30);
                if (k - 2 >= 0) particles[i][j][k]->AddSpringForce(particles[i][j][k - 2], 2*xyzd, 30);
                if (k + 2 < z_size) particles[i][j][k]->AddSpringForce(particles[i][j][k + 2], 2*xyzd, 30);

                if (i + 1 < x_size) {
                    if (k - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i + 1][j][k - 1], dd, 30);
                    if (k + 1 < z_size) particles[i][j][k]->AddSpringForce(particles[i + 1][j][k + 1], dd, 30);
                }
                if (i - 1 >= 0) {
                    if (k - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i - 1][j][k - 1], dd, 30);
                    if (k + 1 < z_size) particles[i][j][k]->AddSpringForce(particles[i - 1][j][k + 1], dd, 30);
                }
                if (j + 1 < y_size) {
                    if (k - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i][j + 1][k - 1], dd, 30);
                    if (k + 1 < z_size) particles[i][j][k]->AddSpringForce(particles[i][j + 1][k + 1], dd, 30);
                }
                if (j - 1 >= 0) {
                    if (k - 1 >= 0) particles[i][j][k]->AddSpringForce(particles[i][j - 1][k - 1], dd, 30);
                    if (k + 1 < z_size) particles[i][j][k]->AddSpringForce(particles[i][j - 1][k + 1], dd, 30);
                }
                 

                for (int ii = i - 1; ii <= i + 1; ii += 2)
                    for (int jj = j - 1; jj <= j + 1; jj += 2)
                        for (int kk = k - 1; kk <= k + 1; kk += 2) {
                            if ((ii >= 0) && (ii >= 0) && (kk >= 0) &&
                                (ii < x_size) && (jj < y_size) && (kk < z_size))
                                particles[i][j][k]->AddSpringForce(particles[ii][jj][kk], cdd, 30);
                        }

                for (int ii = i - 2; ii <= i + 2; ii += 4)
                    for (int jj = j - 2; jj <= j + 2; jj += 4)
                        for (int kk = k - 2; kk <= k + 2; kk += 4) {
                            if ((ii >= 0) && (ii >= 0) && (kk >= 0) &&
                                (ii < x_size) && (jj < y_size) && (kk < z_size))
                                particles[i][j][k]->AddSpringForce(particles[ii][jj][kk], 2*cdd, 30);
                        }


            }
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

    SphereObject * sphere = NewObj< SphereObject>();
    sphere->Init(0, 7, 0, 2, this);
    sphere->method_ = Particle::IntegrationMethod::EulerOrig;
    sphere->bouncing_ = 0.99;
    sphere->friction_ = 0.1;
    sphere->spring_elasticity_k_ = 5;
    sphere->spring_damping_k_ = 1;
    sphere->draw_spring_triangles_ = false;

    is_inited_ = true;
    return 0;
}

int World4::Destroy() {

    is_inited_ = false;
    return true;
}
