#ifndef __Renderer_hpp__
#define __Renderer_hpp__

#include "physics/Types.hpp"
#include "opengl/OpenGLContext.hpp"
#include "opengl/OpenGLRenderer.hpp"
#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    class Renderer {
    public:
        Renderer();

        int Init(OpenGLContext * context);

        bool IsInited();

        int Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model);

        int AddLight(glm::vec3 position, glm::vec3 color);

        int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        bool is_inited_;
        game_engine::OpenGLRenderer * renderer_;

    };

}

#endif