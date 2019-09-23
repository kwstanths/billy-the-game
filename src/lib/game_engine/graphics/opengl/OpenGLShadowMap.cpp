#include "OpenGLShadowMap.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLShadowMap::OpenGLShadowMap() {
        is_inited_ = false;
    }

    OpenGLShadowMap::~OpenGLShadowMap() {
        is_inited_ = false;
    }

    int OpenGLShadowMap::Init(OpenGLContext * context, size_t width, size_t height) {
        context_ = context;
        width_ = width;
        height_ = height;

        glGenFramebuffers(1, &frame_buffer_);
        glGenTextures(1, &output_texture_);
        glBindTexture(GL_TEXTURE_2D, output_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, output_texture_, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            dt::Console(dt::CRITICAL, "Shadow map not complete");
            Unbind();
            return -1;
        }

        Unbind();
        is_inited_ = true;
        return 0;
    }

    int OpenGLShadowMap::Destroy() {
        /* TODO */
        return 0;
    }

    int OpenGLShadowMap::Bind() {

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

        return 0;
    }

    int OpenGLShadowMap::Unbind() {

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return 0;
    }

    void OpenGLShadowMap::ConfigureViewport() {
        glViewport(0, 0, width_, height_);
    }

    void OpenGLShadowMap::ClearDepth() {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

}
}
}