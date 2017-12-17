#ifndef __OpenGLRenderer_hpp__
#define __OpenGLRenderer_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLShaderVariables.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLObject.hpp"

#include "glm/glm.hpp"

namespace game_engine {

    class OpenGLRenderer {
    public:
        /**
        
        */
        OpenGLRenderer();

        /**
        
        */
        int Init(OpenGLContext * context);

        /**
        
        */
        int Destory();

        /**
        
        */
        bool IsInited();

        /**
        
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