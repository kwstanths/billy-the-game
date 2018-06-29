#include "Sun.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace grph = game_engine::graphics;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Sun::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret;
    gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/circle.obj", &ret);
    gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/debug.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);
    gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/map_empty.png", gl::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);

    light_ = ge::graphics::LightProperties_t(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    
    game_time_ = 0.0f;

    /* Normal frequency of one day in seconds */
    day_period_ = 86400.0;

    return ret == 0;
}

void Sun::Step(double delta_time) {

    /* Time passes 400 times faster in the game */
    game_time_ += 2000.0 * delta_time;
    if (game_time_ > day_period_) game_time_ = 0;

    /* Calcluate the hour of the game */
    game_hour_ = game_time_ / 3600.0;
    
    /* 15 degrees for each hour for the 24 hours cycle */
    double color = sin(ge::GetRadians((game_hour_ - 7) * 15));

    light_.diffuse_ = glm::vec3(color, color, color);
    light_.specular_ = glm::vec3(0.5 * color, 0.5 * color, 0.5 * color);
}

void Sun::Draw(grph::Renderer * renderer) {

    renderer->AddDirectionalLight(glm::vec3(0, 0, -1), light_);

    WorldObject::Draw(renderer);
}

double Sun::GetTimeOfDay() {

    return game_hour_;
}

