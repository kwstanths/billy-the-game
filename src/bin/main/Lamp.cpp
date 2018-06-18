#include "Lamp.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Lamp::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret;
    ge::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/circle.obj", &ret);
    ge::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/debug.bmp", ge::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture, x, y, z);

    Scale(0.1f, 0.1f, 0.0f);

    light_intensity_ = glm::vec3(255.0 / 255.0, 214.0 / 255.0, 170.0 / 255.0);

    return ret == 0;
}

void Lamp::Draw(game_engine::Renderer * renderer) {
    
    renderer->AddLight(glm::vec3(GetX(), GetY(), GetZ()), light_intensity_);

    WorldObject::Draw(renderer);
}

