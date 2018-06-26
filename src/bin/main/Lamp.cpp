#include "Lamp.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace grph = game_engine::graphics;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Lamp::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret;
    gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/circle.obj", &ret);
    gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/debug.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);
    gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/map_empty.png", gl::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);

    Scale(0.1f, 0.1f, 0.0f);
    SetCollision(0.05);

    light_ = ge::graphics::LightProperties_t(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.5f, 0.5f, 0.5f));
    center_x_ = x;
    center_y_ = y;
    angular_speed_ = ge::GetRadians(50.0f); /* radians per second */
    radius_ = 3.5;

    return ret == 0;
}

void Lamp::Step(double delta_time) {

    float x = center_x_ + radius_ * cos(angular_speed_ * glfwGetTime());
    float y = center_y_ + radius_ * sin(angular_speed_ * glfwGetTime());
    SetPosition(x, y, GetZ());
}

void Lamp::Draw(grph::Renderer * renderer) {
    
    renderer->AddLight(glm::vec3(GetX(), GetY(), GetZ()+3), light_);

    WorldObject::Draw(renderer);
}

