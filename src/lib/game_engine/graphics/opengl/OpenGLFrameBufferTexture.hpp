#ifndef __OpenGLFrameBufferTexture_hpp__
#define __OpenGLFrameBufferTexture_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLFrameBufferTexture {
    public:
        OpenGLFrameBufferTexture();

        ~OpenGLFrameBufferTexture();

        int Init(OpenGLContext * context, GLint internal_format);

        int Destroy();

        int Bind();

        int Unbind();

        int ClearColor();

        GLuint output_texture_;
    private:
        bool is_inited_;

        OpenGLContext * context_;

        GLuint frame_buffer_;
    };

}
}
}

#endif