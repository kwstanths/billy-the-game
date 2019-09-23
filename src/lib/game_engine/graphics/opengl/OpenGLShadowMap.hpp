#ifndef __OpenGLShadowMap_hpp__
#define __OpenGLShadowMap_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLShadowMap {
    public:
        OpenGLShadowMap();

        ~OpenGLShadowMap();

        int Init(OpenGLContext * context, size_t width, size_t height);

        int Destroy();

        int Bind();

        int Unbind();

        void ConfigureViewport();

        void ClearDepth();

        GLuint output_texture_;
    private:
        bool is_inited_;
        size_t width_;
        size_t height_;

        OpenGLContext * context_;

        GLuint frame_buffer_;
    };

}
}
}

#endif