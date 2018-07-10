#include "Fire.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace mh = game_engine::math;
namespace grph = game_engine::graphics;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include "game_engine/math/RNGenerator.hpp"

bool Fire::Init(float x, float y, float z, game_engine::GameEngine * engine, Sun * sun) {

    int ret;
    //gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/circle.obj", &ret);
    //gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/debug.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);
    //gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/map_empty.png", gl::OpenGLTexture::TEXTURE_STB, &ret);

    //ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);

    //Scale(0.1f, 0.1f, 0.0f);
    //SetCollision(0.05f);

    //light_ = ge::graphics::LightProperties_t(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), glm::vec3(0.4f, 0.4f, 0.4f));
    //att_ = ge::graphics::Attenuation_t(1, 0.02f, 0.0239f);

    //attenutation_noise_ = std::vector<float>(201);
    //mh::RNGenerator gen;
    //gen.Init(0.3f);
    //gen.GetPerlinNoise1d(201, 0.2f, 0.15f, 70, attenutation_noise_);
    //index_ = 0;


    //sun_ = sun;

    return ret == 0;
}

void Fire::Step(double delta_time) {

}

void Fire::Draw(grph::Renderer * renderer) {
    
    double hour = sun_->GetTimeOfDay();

    grph::Attenuation_t att = att_;
    att.linear_ = attenutation_noise_[index_++];
    if (index_ == 200) index_ = 0;

    /* If dark enough, then set the light up */
    if (hour < 8 || hour > 18.5) renderer->AddPointLight(glm::vec3(GetX(), GetY(), GetZ()+0.5), light_, att);
    else renderer->AddPointLight(glm::vec3(GetX(), GetY(), GetZ() + 0.5), ge::graphics::LightProperties_t(0), att_);

    /*renderer->AddLight(glm::vec3(GetX(), GetY(), GetZ() + 0.5), light_, att_);*/

    WorldObject::Draw(renderer);
}

