#ifndef __OpenGLGBuffer_hpp__
#define __OpenGLGBuffer_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLGBuffer {
    public:
        OpenGLGBuffer();

        ~OpenGLGBuffer();

        int Init(OpenGLContext * context);

        int Destroy();

        int Bind();

        int UnBind();

        /* GBuffer textures */
        GLuint g_position_texture_, g_normal_texture_, g_albedo_spec_texture_, g_position_light_;
        GLuint rbo_depth_texture_;

        void ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    private:
        bool is_inited_;

        OpenGLContext * context_;

        GLuint g_buffer_;
        
    };

}
}
}


#endif