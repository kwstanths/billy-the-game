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
    gl::OpenGLTexture * texture = engine->GetAssetManager()->FindTexture("assets/wood.jpg", gl::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, texture, x, y, z);

    SetCollision(1.0f, 1.0f);

    id_ = id;

    return ret == 0;
}

void Treasure::Step(double delta_time){
    
}

void Treasure::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is treasure " + std::to_string(id_) + " speaking");
}
