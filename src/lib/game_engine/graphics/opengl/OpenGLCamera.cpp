#include "OpenGLCamera.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/core/ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include <glm/gtc/matrix_transform.hpp>

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLCamera::OpenGLCamera() {
        is_inited_ = false;
    }

    int OpenGLCamera::Init(OpenGLContext * context)  {
        
        if (context == nullptr) return Error::ERROR_GEN_NOT_INIT;
        if (!context->IsInited()) return Error::ERROR_GEN_NOT_INIT;

        context_ = context;

        if (config_.orthographic_) Ortho2D(config_.zoom_factor_);
        else Project3D();

        is_inited_ = true;
        return 0;
    }

    int OpenGLCamera::Destroy() {
        return 0;
    }

    void OpenGLCamera::GetPositionVector(Real_t & pos_x, Real_t & pos_y, Real_t & pos_z) {
        pos_x = config_.position_.x;
        pos_y = config_.position_.y;
        pos_z = config_.position_.z;
    }

    glm::vec3 OpenGLCamera::GetPositionVector()
    {
        return config_.position_;
    }

    void OpenGLCamera::GetDirectionVector(Real_t & dir_x, Real_t & dir_y, Real_t & dir_z) {
        dir_x = config_.direction_.x;
        dir_y = config_.direction_.y;
        dir_z = config_.direction_.z;
    }

    glm::vec3 OpenGLCamera::GetDirectionVector()
    {
        return config_.direction_;
    }

    void OpenGLCamera::GetUpVector(Real_t & up_x, Real_t & up_y, Real_t & up_z) {
        up_x = config_.up_.x;
        up_y = config_.up_.y;
        up_z = config_.up_.z;
    }

    glm::vec3 OpenGLCamera::GetUpVector()
    {
        return config_.up_;
    }

    Real_t OpenGLCamera::GetPerspectiveAngle() {
        return config_.field_of_view_;
    }

    void OpenGLCamera::SetPositionVector(Real_t x, Real_t y, Real_t z) {
        config_.position_ = glm::vec3(x, y, z);
    }

    void OpenGLCamera::SetDirectionVector(Real_t x, Real_t y, Real_t z) {
        config_.direction_ = glm::vec3(x, y, z);
    }

    void OpenGLCamera::SetUpVector(Real_t x, Real_t y, Real_t z) {
        config_.up_ = glm::vec3(x, y, z);
    }

    void OpenGLCamera::MovePositionVector(Real_t move_x, Real_t move_y, Real_t move_z) {
        if (config_.orthographic_ && !math::Equal(move_z, Real_t(0.0))) {
            dt::Console(dt::WARNING, "OpenGLCamera::MovePositionVector() : Called when orthgraphic projection is \
                set, and z movement factor is significant");
            
            return;
        }

        config_.position_ += glm::vec3(move_x, move_y, move_z);
    }

    void OpenGLCamera::MoveDirectionVector(Real_t move_x, Real_t move_y, Real_t move_z) {
        config_.direction_ += glm::vec3(move_x, move_y, move_z);
    }

    void OpenGLCamera::Zoom(Real_t factor) {
        if (!config_.orthographic_) {
            MovePositionVector(0, 0, factor);
            return;
        }

        config_.zoom_factor_ += factor;

        projection_matrix_ = glm::ortho(
            -1.0 * context_->GetWindowWidth() / config_.zoom_factor_, 1.0 * context_->GetWindowWidth() / config_.zoom_factor_,
            -1.0 * context_->GetWindowHeight() / config_.zoom_factor_, 1.0 * context_->GetWindowHeight() / config_.zoom_factor_,
            static_cast<double>(config_.z_near_), static_cast<double>(config_.z_far_));
    }

    int OpenGLCamera::SetMouceCallback(void(*func)(GLFWwindow *, double, double)) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        glfwSetCursorPosCallback(context_->glfw_window_, func);
        return Error::ERROR_NO_ERROR;
    }

    glm::mat4 OpenGLCamera::GetViewMatrix() {
        return view_matrix_;
    }

    glm::mat4 OpenGLCamera::GetInverseViewMatrix()
    {
        return inverse_view_matrix_;
    }

    glm::mat4 OpenGLCamera::GetProjectionMatrix() {
        return projection_matrix_;
    }

    OpenGLCameraConfig_t OpenGLCamera::GetConfig()
    {
        return config_;
    }

    int OpenGLCamera::CalculateView() {
        if (!is_inited_) return false;

        view_matrix_ = glm::lookAt(config_.position_, config_.position_ + config_.direction_, config_.up_);
        inverse_view_matrix_ = glm::inverse(view_matrix_);

        return 0;
    }

    void OpenGLCamera::Ortho2D(Real_t zoom_factor) {

        config_.zoom_factor_ = zoom_factor;

        projection_matrix_ = glm::ortho(
            -1.0f * context_->GetWindowWidth() / zoom_factor, 1.0f * context_->GetWindowWidth() / zoom_factor,
            -1.0f * context_->GetWindowHeight() / zoom_factor, 1.0f * context_->GetWindowHeight() / zoom_factor,
            static_cast<float>(config_.z_near_), static_cast<float>(config_.z_far_));
    }

    void OpenGLCamera::Project3D() {

        projection_matrix_ = glm::perspective(config_.field_of_view_,
            (1.0f * context_->GetWindowWidth()) / (1.0f * context_->GetWindowHeight()),
            static_cast<float>(config_.z_near_), static_cast<float>(config_.z_far_));
    }

}
}
}