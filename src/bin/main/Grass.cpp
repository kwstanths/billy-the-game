#include "Grass.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Grass::Init(float x, float y, float z, game_engine::GameEngine * engine) {
    
    int ret;
    gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/grass.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);
    gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/map_empty.png", gl::OpenGLTexture::TEXTURE_STB, &ret);

    ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);
    
    Scale(5, 5, 5);

    ge::graphics::Material_t m(glm::vec3(0.1, 0.1, 0.1), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), 2);
    SetMaterial(m);

    return ret == 0;
}

