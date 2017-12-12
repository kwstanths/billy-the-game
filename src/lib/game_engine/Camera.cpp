#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace game_engine {

    Camera::Camera() {

    }

    int Camera::Init(float pos_x, float pos_y, float pos_z, 
        float dir_x, float dir_y, float dir_z,
        float up_x, float up_y, float up_z) 
    {
        position_ = glm::vec3(pos_x, pos_y, pos_z);
        direction_ = glm::vec3(dir_x, dir_y, dir_z);
        up_ = glm::vec3(up_x, up_y, up_z);
        
        return 0;
    }

    int Camera::Destroy() {
        return -1;
    }

    glm::mat4 Camera::GetView() {
        glm::mat4 View = glm::lookAt(position_, direction_, up_);

        return View;
    }

}