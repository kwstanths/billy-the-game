#include "Wall.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Wall::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret;
    gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/wall.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);
    gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/map_empty.png", gl::OpenGLTexture::TEXTURE_STB, &ret);
    
    ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);

    SetCollision(1.0f, 1.0f);

    ge::graphics::Material_t m(GAME_ENGINE_MATERIAL_JADE);
    m.shininess_ = 32;
    SetMaterial(m);

    return ret == 0;
}

void Wall::Step(double delta_time) {

}

void Wall::Interact() {
    dt::Console(dt::INFO, "Hello? Yes? This is wall speaking. Let's destroy ths object");

    Destroy();
}
