#include "Sun.hpp"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/graphics/Material.hpp"
#include "game_engine/graphics/opengl/OpenGLCamera.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;
namespace grph = game_engine::graphics;


bool Sun::Init(ge::Real_t x, ge::Real_t y, ge::Real_t z, ge::WorldSector * world, ge::GameEngine * engine) {

    int ret = WorldObject::Init("sphere.obj", x, y, z, true);
    world->AddObject(this, x, y, z);

    Scale(0.1, 0.1, 0.1);

    DirectionalLight::direction_ = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));
    DirectionalLight::ambient_ = glm::vec3(0.2f, 0.2f, 0.2f);
    DirectionalLight::diffuse_ = glm::vec3(0.5f, 0.5f, 0.5f);
    DirectionalLight::specular_ = glm::vec3(0.3f, 0.3f, 0.3f);
    world->SetDirectionalLight(this);
    engine->GetRenderer()->SetLightShadows(this);

    engine_ = engine;

    return ret == 0;
}

void Sun::Step(double delta_time) {

}

void Sun::Draw(grph::Renderer * renderer) {

    WorldObject::Draw(renderer);
}
