#ifndef __Renderer_hpp__
#define __Renderer_hpp__

#include "game_engine/physics/Types.hpp"
#include "game_engine/graphics/opengl/OpenGLContext.hpp"
#include "game_engine/graphics/opengl/OpenGLRenderer.hpp"
#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/opengl/OpenGLCamera.hpp"

#include "Material.hpp"
#include "GraphicsObject.hpp"

namespace game_engine {
namespace graphics {

    class Renderer {
    public:
        Renderer();

        int Init(opengl::OpenGLContext * context);

        bool IsInited();

        void SetView(opengl::OpenGLCamera * camera);

        int Draw(GraphicsObject * rendering_object);

        int Draw(opengl::OpenGLObject * object, opengl::OpenGLTexture * texture, glm::mat4 model);

        int AddLight(glm::vec3 position, LightProperties_t light_properties);

        int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        bool is_inited_;
        opengl::OpenGLRenderer * renderer_;

    };

}
}

#endif