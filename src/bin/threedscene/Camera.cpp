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

void MouseCallbackNONE(GLFWwindow * w, double x, double y) { 

};

Camera::Camera(int width, int height, ge::Real_t mouse_sensitivity) : OpenGLCamera() {
    /* Hand picked values */
    /* Configure OpenGLCamera parameters */
    config_.position_ = glm::vec3(-8.7, 10.5, 14.48);
    config_.direction_ = glm::vec3(0.4818, -0.439, -0.7578);
    /* Configure OpenGLCamera parameters */
    config_.up_ = glm::vec3(0, 1, 0);
    config_.orthographic_ = false;
    config_.zoom_factor_ = 75;
    config_.z_near_ = 0.1;
    config_.z_far_ = 600;

    /* Configure rest */
    yaw_ = -56.75f;
    pitch_ = -20.4f;

    mouse_sensitivity_ = mouse_sensitivity;

    last_cursor_position_x_ = (1.0f *width) / 2.0f;
    last_cursor_position_y_ = (1.0f * height) / 2.0f;

    first_mouse_call_ = true;
}

void Camera::KeyboardMove(ge::Real_t x, ge::Real_t y, ge::Real_t z) {
    MovePositionVector(x, y, z);
}

void Camera::KeyboardMoveFlightMode(ge::Real_t front, ge::Real_t left) {
    /* Get current direction */
    glm::vec3 curr_direction;
    GetDirectionVector(curr_direction.x, curr_direction.y, curr_direction.z);
    /* Get current position */
    glm::vec3 curr_position;
    GetPositionVector(curr_position.x, curr_position.y, curr_position.z);

    /* Calculate the amount of front movement */
    glm::vec3 new_position = curr_position + front * curr_direction;

    /* Calculate the left vector */
    glm::vec3 up_vector;
    GetUpVector(up_vector.x, up_vector.y, up_vector.z);
    glm::vec3 left_vector = glm::cross(curr_direction, up_vector);

    /* Calculate the amount of left movement */
    new_position = new_position + left_vector * left;

    /* Set new position */
    SetPositionVector(new_position.x, new_position.y, new_position.z);
}

void Camera::MouseMove(ge::Real_t mouse_x, ge::Real_t mouse_y) {

    if (first_mouse_call_) {
        last_cursor_position_x_ = mouse_x;
        last_cursor_position_y_ = mouse_y;
        first_mouse_call_ = false;
    }

    ge::Real_t xoffset = mouse_x - last_cursor_position_x_;
    ge::Real_t yoffset = last_cursor_position_y_ - mouse_y;
    last_cursor_position_x_ = mouse_x;
    last_cursor_position_y_ = mouse_y;

    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

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
    GetPositionVector(prev_x, prev_y, prev_z);    

    SetPositionVector(x, y, prev_z);
}
