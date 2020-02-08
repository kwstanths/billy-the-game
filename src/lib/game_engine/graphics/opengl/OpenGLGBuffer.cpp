#include "OpenGLGBuffer.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLGBuffer::OpenGLGBuffer() {
        is_inited_ = false;
    }

    OpenGLGBuffer::~OpenGLGBuffer() {
        is_inited_ = false;
    }

    int OpenGLGBuffer::Init(OpenGLContext * context) {
        context_ = context;

        /* Generate a frame buffer object */
        glGenFramebuffers(1, &g_buffer_);
        glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_);

        /* Generate the render targets */
        /* Store position */
        glGenTextures(1, &g_position_texture_);
        glBindTexture(GL_TEXTURE_2D, g_position_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, context_->GetWindowWidth(), context_->GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position_texture_, 0);

        /* Store normal */
        glGenTextures(1, &g_normal_texture_);
        glBindTexture(GL_TEXTURE_2D, g_normal_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, context_->GetWindowWidth(), context_->GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal_texture_, 0);

        /* Store albedo (diffuse and specular component), The A component will be the specular intensity */
        glGenTextures(1, &g_albedo_spec_texture_);
        glBindTexture(GL_TEXTURE_2D, g_albedo_spec_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, context_->GetWindowWidth(), context_->GetWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo_spec_texture_, 0);

        /* Store ambient color and shadow visibility */
        glGenTextures(1, &g_position_light_);
        glBindTexture(GL_TEXTURE_2D, g_position_light_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, context_->GetWindowWidth(), context_->GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_position_light_, 0);

        /* Configure the render targets */
        unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(4, attachments);

        /* Create a depth buffer */
        glGenRenderbuffers(1, &rbo_depth_texture_);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_texture_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, context_->GetWindowWidth(), context_->GetWindowHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_texture_);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            dt::Console(dt::CRITICAL, "GBuffer not complete");
            UnBind();
            return -1;
        }

        UnBind();
        is_inited_ = true;
        return 0;
    }

    int OpenGLGBuffer::Destroy() {
        /* TODO */
        return 0;
    }

    int OpenGLGBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_);

        return 0;
    }

    int OpenGLGBuffer::UnBind() {
        /* Unbind frame buffer */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return 0;
    }

}
}
}