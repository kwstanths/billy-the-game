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

    OpenGLCShadowMapS::OpenGLCShadowMapS() {
        is_inited_ = false;
    }

    OpenGLCShadowMapS::~OpenGLCShadowMapS() {
        is_inited_ = false;
    }

    int OpenGLCShadowMapS::Init(OpenGLContext * context, size_t width, size_t height) {
        width_ = width;
        height_ = height;
        aspect_ratio_ = (float)context->GetWindowWidth() / (float)context->GetWindowHeight();
        
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

    int OpenGLCShadowMapS::Destroy() {
        /* TODO */
        return 0;
    }

    int OpenGLCShadowMapS::Bind(size_t index) {

        if (index >= n_shadow_maps_) return -1;

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_maps[index], 0);

        return 0;
    }

    int OpenGLCShadowMapS::Unbind() {

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return 0;
    }

    void OpenGLCShadowMapS::CalculateProjectionMatrices(glm::vec3 light_direction, OpenGLCamera * camera)
    {
        /* Calculate an orthogonal coordinate system based on the light direction */
        glm::vec3 light_forward = light_direction;
        glm::vec3 light_side = glm::vec3(light_forward.y, -light_forward.x, 0);
        glm::vec3 light_up = glm::cross(light_forward, light_side);

        /* Get camera info */
        glm::mat4 view_space_inverse = camera->GetInverseViewMatrix();
        OpenGLCameraConfig_t camera_config = camera->GetConfig();

        GLfloat fov = camera_config.field_of_view_;
        float tanHalfHFOV = tanf(math::GetRadians(fov / 2.0f));
        float tanHalfVFOV = tanf(math::GetRadians((fov * aspect_ratio_) / 2.0f));

        GLfloat z_near = camera_config.z_near_;
        GLfloat z_far = camera_config.z_far_;

        /* Calculate the limits of the cascades */
        cascade_limits_[0] = camera_config.z_near_;
        cascade_limits_[1] = camera_config.z_near_ + 0.14 * (z_far - z_near);
        cascade_limits_[2] = camera_config.z_near_ + 0.4 * (z_far - z_near);
        cascade_limits_[3] = camera_config.z_far_;

        /* For all cascades */
        for (size_t i = 0; i < n_shadow_maps_; i++) {
            
            /* Caclulate the 8 points of that cascade's frustum in view space */
            float xn = cascade_limits_[i] * tanHalfHFOV;
            float xf = cascade_limits_[i + 1] * tanHalfHFOV;
            float yn = cascade_limits_[i] * tanHalfVFOV;
            float yf = cascade_limits_[i + 1] * tanHalfVFOV;

            glm::vec4 frustum_corners_view_space[8] = {
                // near face
                glm::vec4(xn, yn, -cascade_limits_[i], 1.0),
                glm::vec4(-xn, yn, -cascade_limits_[i], 1.0),
                glm::vec4(xn, -yn, -cascade_limits_[i], 1.0),
                glm::vec4(-xn, -yn, -cascade_limits_[i], 1.0),

                // far face
                glm::vec4(xf, yf, -cascade_limits_[i + 1], 1.0),
                glm::vec4(-xf, yf, -cascade_limits_[i + 1], 1.0),
                glm::vec4(xf, -yf, -cascade_limits_[i + 1], 1.0),
                glm::vec4(-xf, -yf, -cascade_limits_[i + 1], 1.0)
            };
   
            /* Transform them to world space, and calculate a center and a radius for a bounding sphere covering the frustum */
            /* There is probably a faster way to do this, but oh well, the points might be usefull in later optimizations */
            glm::vec3 frustum_bs_center(0, 0, 0);
            glm::vec4 frustum_corners_world_space[8];
            for (size_t j = 0; j < 8; j++) {    
                frustum_corners_world_space[j] = view_space_inverse * frustum_corners_view_space[j];
                frustum_bs_center += glm::vec3(frustum_corners_world_space[j]) / 8.0f;
            }
            float frustum_bs_radius = glm::distance(frustum_bs_center, glm::vec3(frustum_corners_world_space[4]));

            /* 
                Fix sphere center to avoid flickering:
                [shaderx7] 4.1 Practical Cascaded Shadow Maps
                https://pt.slideshare.net/ohyecloudy/shaderx7-41-practical-cascaded-shadow-maps?nomobile=true
            */
            float x = glm::ceil(glm::dot(frustum_bs_center, light_up) * 1024 / frustum_bs_radius) * frustum_bs_radius / 1024;
            float y = glm::ceil(glm::dot(frustum_bs_center, light_side) * 1024/ frustum_bs_radius) * frustum_bs_radius / 1024;
            frustum_bs_center = light_up * x + light_side * y + light_direction * glm::dot(frustum_bs_center, light_direction);

            /* Calculate view and projection matrices */
            view_matrices_[i] = glm::lookAt(
                frustum_bs_center - light_direction * frustum_bs_radius,
                glm::vec3(frustum_bs_center),
                light_up);

            projection_matrices_[i] = glm::ortho(-frustum_bs_radius, frustum_bs_radius, -frustum_bs_radius, frustum_bs_radius, 0.0f, 2 * frustum_bs_radius);
            
            lightspace_[i] = projection_matrices_[i] * view_matrices_[i];
        }
    }

    void OpenGLCShadowMapS::ActivateTextures(size_t index)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, shadow_maps[0]);

        glActiveTexture(GL_TEXTURE0 + index + 1);
        glBindTexture(GL_TEXTURE_2D, shadow_maps[1]);

        glActiveTexture(GL_TEXTURE0 + index + 2);
        glBindTexture(GL_TEXTURE_2D, shadow_maps[2]);
    }

    void OpenGLCShadowMapS::ConfigureViewport() {
        glViewport(0, 0, width_, height_);
    }

    void OpenGLCShadowMapS::ClearDepth() {
        Bind(0);
        glClear(GL_DEPTH_BUFFER_BIT);
        Bind(1);
        glClear(GL_DEPTH_BUFFER_BIT);
        Bind(2);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    size_t OpenGLCShadowMapS::GetNCascades() {
        return n_shadow_maps_;
    }

    glm::mat4 & OpenGLCShadowMapS::GetProjectionMatrix(size_t index) {
        return projection_matrices_[index];
    }

    glm::mat4 & OpenGLCShadowMapS::GetViewMatrix(size_t index)
    {
        return view_matrices_[index];
    }

    glm::mat4 & OpenGLCShadowMapS::GetLightspaceMatrix(size_t index)
    {
        return lightspace_[index];
    }

    float OpenGLCShadowMapS::GetCascadeEnd(size_t index)
    {
        return cascade_limits_[index + 1];
    }

}
}
}