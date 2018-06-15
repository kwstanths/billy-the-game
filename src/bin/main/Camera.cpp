#include "Camera.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

Camera::Camera(int width, int height, float mouse_sensitivity) : OpenGLCamera() {

    yaw_ = -90.0f;
    pitch_ = 0.0f;

    mouse_sensitivity_ = mouse_sensitivity;

    last_cursor_position_x_ = (1.0f *width) / 2.0f;
    last_cursor_position_y_ = (1.0f * height) / 2.0f;
}

void Camera::KeyboardMove(float x, float y, float z) {
    MovePositionVector(x, y, z);
}

void Camera::MouseMove(float mouse_x, float mouse_y) {
    float xoffset = mouse_x - last_cursor_position_x_;
    float yoffset = last_cursor_position_y_ - mouse_y;
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
