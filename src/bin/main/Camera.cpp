#include "Camera.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;

/* Camera needs to be global, since the mouse callback needs to be global */
Camera * camera;

void MouseCallback(GLFWwindow * w, double x, double y) {
    camera->MouseMove(static_cast<ge::Real_t>(x), static_cast<ge::Real_t>(y));
}

void MouseCallbackNONE(GLFWwindow * w, double x, double y) { };

Camera::Camera(int width, int height, ge::Real_t mouse_sensitivity) : OpenGLCamera() {

    z_height_ = 15;
    /* Configure OpenGLCamera parameters */
    config_.position_ = glm::vec3(0, 0, z_height_);
    config_.direction_ = glm::vec3(0, 0, -1);
    config_.up_ = glm::vec3(0, 1, 0);
    config_.orthographic_ = false;
    config_.zoom_factor_ = 75;

    /* Configure rest */
    yaw_ = -90.0f;
    pitch_ = 0.0f;

    mouse_sensitivity_ = mouse_sensitivity;

    last_cursor_position_x_ = (1.0f *width) / 2.0f;
    last_cursor_position_y_ = (1.0f * height) / 2.0f;

}

void Camera::KeyboardMove(ge::Real_t x, ge::Real_t y, ge::Real_t z) {
    MovePositionVector(x, y, z);
}

void Camera::MouseMove(ge::Real_t mouse_x, ge::Real_t mouse_y) {
    ge::Real_t xoffset = mouse_x - last_cursor_position_x_;
    ge::Real_t yoffset = last_cursor_position_y_ - mouse_y;
    last_cursor_position_x_ = mouse_x;
    last_cursor_position_y_ = mouse_y;

    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
    direction.y = sin(glm::radians(pitch_));
    direction.z = cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));
    direction = glm::normalize(direction);
    SetDirectionVector(direction.x, direction.y, direction.z);
}

void Camera::Set2DPosition(ge::Real_t x, ge::Real_t y) {

    /* Get previous height values */
    ge::Real_t prev_x, prev_y, prev_z;
    GetPositionVector(&prev_x, &prev_y, &prev_z);    

    SetPositionVector(x, y, prev_z);
}
