#ifndef __OpenGLRenderer_hpp__
#define __OpenGLRenderer_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLShaders.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLObject.hpp"

#include "glm/glm.hpp"

namespace game_engine {

    class OpenGLRenderer {
    public:
        /**
            Does nothing in particular
        */
        OpenGLRenderer();

        /**
            Initializes the renderer
            @parma context An OpenGL context, mainly for the shader variables
            @return 0=OK
        */
        int Init(OpenGLContext * context);

        /**
            Destroys allocated data
            @return 0=OK
        */
        int Destroy();

        /**
            Return wether the object is initialised
        */
        bool IsInited();

        /**
            Draw an object on the scene
            @param object The object to be drawn
            @param texture The texture to draw
            @param model The object's model
            @return 0=OK, -1=Something's not initialised;
        */
        int Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model);
        

    private:
        bool is_inited_;
        OpenGLContext * context_;
        OpenGLShaderVariables_t shader_vars_;
        
        GLuint VAO_;
    };

}

#endif