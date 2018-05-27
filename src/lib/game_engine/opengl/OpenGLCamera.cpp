#include "OpenGLCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "../physics/HelpFunctions.hpp"

#include "debug_tools/Console.hpp"

namespace dt = debug_tools;

namespace game_engine {

    OpenGLCamera::OpenGLCamera(OpenGLCameraConfig_t config) {
        is_inited_ = false;

        config_ = config;
    }

    int OpenGLCamera::Init(OpenGLContext * context)  {
        context_ = context;

        shader_vars_ = context->GetShaderVariables();

        glm::vec3 position = glm::vec3(config_.pos_x_, config_.pos_y_, config_.pos_z_);
        glm::vec3 direction = glm::vec3(config_.dir_x_, config_.dir_y_, config_.dir_z_);
        glm::vec3 up = glm::vec3(config_.up_x_, config_.up_y_, config_.up_z_);
        
        view_matrix_ = glm::lookAt(position, direction, up);

        if (config_.orthographic_) Ortho2D(config_.zoom_factor_);
        else Project3D();
        
        is_inited_ = true;
        return 0;
    }

    int OpenGLCamera::Destroy() {
        return 0;
    }

    void OpenGLCamera::SetPosition(OpenGLCameraConfig_t params) {
        config_ = params;

        glm::vec3 position = glm::vec3(config_.pos_x_, config_.pos_y_, config_.pos_z_);
        glm::vec3 direction = glm::vec3(config_.dir_x_, config_.dir_y_, config_.dir_z_);
        glm::vec3 up = glm::vec3(config_.up_x_, config_.up_y_, config_.up_z_);

        view_matrix_ = glm::lookAt(position, direction, up);
    }

    void OpenGLCamera::GetPosition(float * pos_x, float * pos_y, float * pos_z) {
        *pos_x = config_.pos_x_;
        *pos_y = config_.pos_y_;
        *pos_z = config_.pos_z_;
    }

    void OpenGLCamera::Move(float move_x, float move_y, float move_z) {
        if (config_.orthographic_ && !Equal(move_z, 0.0f)) {
            dt::Console(dt::WARNING, "OpenGLCamera::Move() : Called when orthgraphic projection is \
                set, and z movement factor is significant");
            
            return;
        }

        glm::vec3 position = glm::vec3(config_.pos_x_ += move_x, config_.pos_y_ += move_y, config_.pos_z_ += move_z);
        glm::vec3 direction = glm::vec3(config_.dir_x_ += move_x, config_.dir_y_ += move_y, config_.dir_z_ += move_z);
        glm::vec3 up = glm::vec3(config_.up_x_, config_.up_y_, config_.up_z_);

        view_matrix_ = glm::lookAt(position, direction, up);
    }

    void OpenGLCamera::Zoom(float factor) {
        if (!config_.orthographic_) {
            Move(0, 0, factor);
            return;
        }

        config_.zoom_factor_ += factor;

        projection_matrix_ = glm::ortho(
            -1.0f * context_->GetWindowWidth() / config_.zoom_factor_,  1.0f * context_->GetWindowWidth() / config_.zoom_factor_,
            -1.0f * context_->GetWindowHeight() / config_.zoom_factor_, 1.0f * context_->GetWindowHeight() / config_.zoom_factor_,
            -10.0f, 10.0f);
    }

    int OpenGLCamera::SetView() {
        if (!is_inited_) return false;

        glUniformMatrix4fv(shader_vars_.uni_View_, 1, GL_FALSE, &(view_matrix_[0][0]));
        glUniformMatrix4fv(shader_vars_.uni_Projection_, 1, GL_FALSE, &(projection_matrix_[0][0]));

        return 0;
    }

    void OpenGLCamera::Ortho2D(float zoom_factor) {

        projection_matrix_ = glm::ortho(
            -1.0f * context_->GetWindowWidth() / zoom_factor, 1.0f * context_->GetWindowWidth() / zoom_factor,
            -1.0f * context_->GetWindowHeight() / zoom_factor, 1.0f * context_->GetWindowHeight() / zoom_factor,
            -10.0f, 10.0f);
    }

    void OpenGLCamera::Project3D() {
     
        projection_matrix_ = glm::perspective(glm::radians(45.0f),
            (1.0f * context_->GetWindowWidth()) / (1.0f * context_->GetWindowHeight()),
            0.1f, 100.0f);
    }

}