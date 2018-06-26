#include "Treasure.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

bool Treasure::Init(float x, float y, float z, ge::GameEngine * engine, size_t id) {
    
    int ret;
    gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/container.png", gl::OpenGLTexture::TEXTURE_STB, &ret);
    gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/container_spec_map.png", gl::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);

    SetCollision(1.0f, 1.0f);

    ge::graphics::Material_t m(glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.54, 0.27, 0.07), glm::vec3(0.1, 0.1, 0.1), 32);
    SetMaterial(m);

    id_ = id;

    return ret == 0;
}

void Treasure::Step(double delta_time){
    
}

void Treasure::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is treasure " + std::to_string(id_) + " speaking");
}
