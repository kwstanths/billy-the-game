#include "Path.hpp"

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


bool Path::Init(float x, float y, float z, game_engine::GameEngine * engine) {

    int ret;
    //gl::OpenGLObject * object = engine->GetAssetManager()->FindObject("assets/tile.obj", &ret);
    //gl::OpenGLTexture * diffuse_texture = engine->GetAssetManager()->FindTexture("assets/path.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);
    //gl::OpenGLTexture * specular_texture = engine->GetAssetManager()->FindTexture("assets/map_empty.png", gl::OpenGLTexture::TEXTURE_STB, &ret);

    //ret = WorldObject::Init(object, diffuse_texture, specular_texture, x, y, z);

    //Scale(5, 5, 0);

    //ge::graphics::Material_t m(glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.96, 0.90, 0.70), glm::vec3(0.1, 0.1, 0.1), 32);
    //SetMaterial(m);

    return ret == 0;
}

