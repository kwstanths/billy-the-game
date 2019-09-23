#include "OpenGLFrameBufferTexture.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {
namespace graphics {
namespace opengl {


    OpenGLFrameBufferTexture::OpenGLFrameBufferTexture() {
        is_inited_ = false;
    }

    OpenGLFrameBufferTexture::~OpenGLFrameBufferTexture() {
        is_inited_ = false;
    }

    int OpenGLFrameBufferTexture::Init(OpenGLContext * context, GLint internal_format) {
        context_ = context;

        glGenFramebuffers(1, &frame_buffer_);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
        glGenTextures(1, &output_texture_);
        glBindTexture(GL_TEXTURE_2D, output_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, context_->GetWindowWidth(), context_->GetWindowHeight(), 0, internal_format, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_texture_, 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        
        /* Clear color */
        GLuint clear_color[4] = { 0, 0, 0, 0 };
        glClearBufferuiv(GL_COLOR, 0, clear_color);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            dt::Console(dt::CRITICAL, "Texture frame buffer not complete");
            Unbind();
            return -1;
        }

        Unbind();
        is_inited_ = true;
        return 0;
    }

    int OpenGLFrameBufferTexture::Destroy() {
        /* TODO */
        return 0;
    }

    int OpenGLFrameBufferTexture::Bind() {

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

        return 0;
    }

    int OpenGLFrameBufferTexture::Unbind() {

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return 0;
    }

    int OpenGLFrameBufferTexture::ClearColor() {

        Bind();
        GLuint clear_color[4] = { 0, 0, 0, 0 };
        glClearBufferuiv(GL_COLOR, 0, clear_color);
        Unbind();

        return 0;
    }

}
}
}