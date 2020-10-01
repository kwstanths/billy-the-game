#include "OpenGLShadowMap.hpp"

#undef min
#undef max

#include <limits>
#include <algorithm>

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

    int OpenGLShadowMap::Init(size_t width, size_t height) {
        width_ = width;
        height_ = height;
        
        glGenFramebuffers(1, &frame_buffer_);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

        glGenTextures(3, shadow_maps);
        for (size_t i = 0; i < n_shadow_maps_; i++) {
            glBindTexture(GL_TEXTURE_2D, shadow_maps[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_maps[0], 0);
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

    int OpenGLShadowMap::Bind(size_t index) {

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_maps[index], 0);

        return 0;
    }

    int OpenGLShadowMap::Unbind() {

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return 0;
    }

    void OpenGLShadowMap::CalculateProjectionMatrices(glm::vec3 light_direction, OpenGLCamera * camera)
    {
        glm::mat4 view_space_inverse = camera->GetInverseViewMatrix();
        OpenGLCameraConfig_t camera_config = camera->GetConfig();

        GLfloat fov = camera_config.field_of_view_;
        float aspect = 1920.0 / 1080.0;
        float tanHalfHFOV = tanf(math::GetRadians(fov / 2.0f));
        float tanHalfVFOV = tanf(math::GetRadians((fov * aspect) / 2.0f));

        GLfloat z_near = camera_config.z_near_;
        GLfloat z_far = camera_config.z_far_;

        GLfloat cascade_end[4];
        cascade_end[0] = camera_config.z_near_;
        cascade_end[1] = camera_config.z_near_ + 0.2 * (z_far - z_near);
        cascade_end[2] = camera_config.z_near_ + 0.4 * (z_far - z_near);
        cascade_end[3] = camera_config.z_far_;

        for (size_t i = 0; i < n_shadow_maps_; i++) {
            float xn = cascade_end[i] * tanHalfHFOV;
            float xf = cascade_end[i + 1] * tanHalfHFOV;
            float yn = cascade_end[i] * tanHalfVFOV;
            float yf = cascade_end[i + 1] * tanHalfVFOV;

            glm::vec4 frustum_corners_view_space[8] = {
                // near face
                glm::vec4(xn, yn, -cascade_end[i], 1.0),
                glm::vec4(-xn, yn, -cascade_end[i], 1.0),
                glm::vec4(xn, -yn, -cascade_end[i], 1.0),
                glm::vec4(-xn, -yn, -cascade_end[i], 1.0),

                // far face
                glm::vec4(xf, yf, -cascade_end[i + 1], 1.0),
                glm::vec4(-xf, yf, -cascade_end[i + 1], 1.0),
                glm::vec4(xf, -yf, -cascade_end[i + 1], 1.0),
                glm::vec4(-xf, -yf, -cascade_end[i + 1], 1.0)
            };
   
            glm::vec4 frustum_center(0, 0, 0, 0);
            glm::vec4 frustum_corners_world_space[8];
            for (size_t j = 0; j < 8; j++) {    
                frustum_corners_world_space[j] = view_space_inverse * frustum_corners_view_space[j];
                frustum_center += frustum_corners_world_space[j] / 8.0f;
            }

            float minX = std::numeric_limits<float>::max();
            float maxX = -std::numeric_limits<float>::max();
            float minY = std::numeric_limits<float>::max();
            float maxY = -std::numeric_limits<float>::max();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = -std::numeric_limits<float>::max();

            glm::vec4 frustum_corners_light_space[8];
            for (size_t j = 0; j < 8; j++) {
                frustum_corners_light_space[j] = view_matrices_[i] * frustum_corners_world_space[j];

                minX = std::min(minX, frustum_corners_light_space[j].x);
                maxX = std::max(maxX, frustum_corners_light_space[j].x);
                minY = std::min(minY, frustum_corners_light_space[j].y);
                maxY = std::max(maxY, frustum_corners_light_space[j].y);
                minZ = std::min(minZ, frustum_corners_light_space[j].z);
                maxZ = std::max(maxZ, frustum_corners_light_space[j].z);
            }

            glm::vec3 forward = light_direction;
            glm::vec3 a = glm::vec3(forward.y, -forward.x, 0);
            view_matrices_[i] = glm::lookAt(
                glm::vec3(frustum_center) - light_direction * (cascade_end[i+1] - cascade_end[i]),
                glm::vec3(frustum_center),
                a);

            projection_matrices_[i] = glm::ortho(minX, maxX, minY, maxY, 0.0f, 1.5f * (maxZ - minZ));
            
            lightspace_[i] = projection_matrices_[i] * view_matrices_[i];
        }
    }

    int OpenGLShadowMap::ActivateTextures(size_t index)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, shadow_maps[0]);

        glActiveTexture(GL_TEXTURE0 + index + 1);
        glBindTexture(GL_TEXTURE_2D, shadow_maps[1]);

        glActiveTexture(GL_TEXTURE0 + index + 2);
        glBindTexture(GL_TEXTURE_2D, shadow_maps[2]);

        return 0;
    }

    void OpenGLShadowMap::ConfigureViewport() {
        glViewport(0, 0, width_, height_);
    }

    void OpenGLShadowMap::ClearDepth() {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    size_t OpenGLShadowMap::GetNCascades() {
        return n_shadow_maps_;
    }

    glm::mat4 & OpenGLShadowMap::GetProjectionMatrix(size_t index) {
        return projection_matrices_[index];
    }

    glm::mat4 & OpenGLShadowMap::GetViewMatrix(size_t index)
    {
        return view_matrices_[index];
    }

    glm::mat4 & OpenGLShadowMap::GetLightspaceMatrix(size_t index)
    {
        return lightspace_[index];
    }

}
}
}