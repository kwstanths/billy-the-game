#include "Fire.hpp"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Fire::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init();
    world->AddObject(this, x, y, z);

    Scale(0.05f, 0.05f, 0.05f);

    DirectionalLight::direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
    DirectionalLight::ambient_ = glm::vec3(0.4f, 0.4f, 0.4f);
    DirectionalLight::diffuse_ = glm::vec3(0.7f, 0.7f, 0.7f);
    DirectionalLight::specular_ = glm::vec3(0.2f, 0.2f, 0.2f);
    world->SetDirectionalLight(this);

    return ret == 0;
}

void Fire::Step(double delta_time) {

}

void Fire::Draw(grph::Renderer * renderer) {

    /* Set shadow map variables */
    float scene_size = 20.0;
    float near_plane = 1.0f, far_plane = 5;

    glm::mat4 lightProjection = glm::ortho(-scene_size, scene_size, -scene_size, scene_size, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(20.0f, 3.0f, 20.0f),
        glm::vec3(20.0f, 0.0f, 20.0f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    renderer->SetShadowMap(lightView, lightProjection);

    WorldObject::Draw(renderer);
}
