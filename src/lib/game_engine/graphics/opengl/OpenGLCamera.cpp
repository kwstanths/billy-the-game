#include "OpenGLCamera.hpp"

#include "game_engine/physics/HelpFunctions.hpp"
#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include <glm/gtc/matrix_transform.hpp>

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLCamera::OpenGLCamera() {
        is_inited_ = false;
    }

    int OpenGLCamera::Init(OpenGLCameraConfig_t config, OpenGLContext * context)  {
        
        config_ = config;
        context_ = context;

        if (config.orthographic_) Ortho2D(config.zoom_factor_);
        else Project3D();

        is_inited_ = true;
        return 0;
    }

    int OpenGLCamera::Destroy() {
        return 0;
    }

    void OpenGLCamera::GetPositionVector(float * pos_x, float * pos_y, float * pos_z) {
        *pos_x = config_.position_.x;
        *pos_y = config_.position_.y;
        *pos_z = config_.position_.z;
    }

    void OpenGLCamera::GetDirectionVector(float * dir_x, float * dir_y, float * dir_z) {
        *dir_x = config_.direction_.x;
        *dir_y = config_.direction_.y;
        *dir_z = config_.direction_.z;
    }

    void OpenGLCamera::GetUpVector(float * up_x, float * up_y, float * up_z) {
        *up_x = config_.up_.x;
        *up_y = config_.up_.y;
        *up_z = config_.up_.z;
    }

    void OpenGLCamera::SetPositionVector(float x, float y, float z) {
        config_.position_ = glm::vec3(x, y, z);
    }

    void OpenGLCamera::SetDirectionVector(float x, float y, float z) {
        config_.direction_ = glm::vec3(x, y, z);
    }

    void OpenGLCamera::SetUpVector(float x, float y, float z) {
        config_.up_ = glm::vec3(x, y, z);
    }

    void OpenGLCamera::MovePositionVector(float move_x, float move_y, float move_z) {
        if (config_.orthographic_ && !Equal(move_z, 0.0f)) {
            dt::Console(dt::WARNING, "OpenGLCamera::MovePositionVector() : Called when orthgraphic projection is \
                set, and z movement factor is significant");
            
            return;
        }

        config_.position_ += glm::vec3(move_x, move_y, move_z);
    }

    void OpenGLCamera::MoveDirectionVector(float move_x, float move_y, float move_z) {
        config_.direction_ += glm::vec3(move_x, move_y, move_z);
    }

    void OpenGLCamera::Zoom(float factor) {
        if (!config_.orthographic_) {
            MovePositionVector(0, 0, factor);
            return;
        }

        config_.zoom_factor_ += factor;

        projection_matrix_ = glm::ortho(
            -1.0f * context_->GetWindowWidth() / config_.zoom_factor_,  1.0f * context_->GetWindowWidth() / config_.zoom_factor_,
            -1.0f * context_->GetWindowHeight() / config_.zoom_factor_, 1.0f * context_->GetWindowHeight() / config_.zoom_factor_,
            -10.0f, 10.0f);
    }

    int OpenGLCamera::SetMouceCallback(void(*func)(GLFWwindow *, double, double)) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        glfwSetCursorPosCallback(context_->glfw_window_, func);
        return Error::ERROR_NO_ERROR;
    }

    int OpenGLCamera::CalculateView() {
        if (!is_inited_) return false;

        view_matrix_ = glm::lookAt(config_.position_, config_.position_ + config_.direction_, config_.up_);

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
}
}