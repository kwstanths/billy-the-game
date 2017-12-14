#include "OpenGLCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace game_engine {

    OpenGLCamera::OpenGLCamera(OpenGLCameraParams_t params) {
        is_inited_ = false;

        params_ = params;
    }

    int OpenGLCamera::Init(OpenGLContext * context) 
    {
        shader_vars_ = context->GetShaderVariables();

        glm::vec3 position = glm::vec3(params_.pos_x_, params_.pos_y_, params_.pos_z_);
        glm::vec3 direction = glm::vec3(params_.dir_x_, params_.dir_y_, params_.dir_z_);
        glm::vec3 up = glm::vec3(params_.up_x_, params_.up_y_, params_.up_z_);
        
        view_matrix_ = glm::lookAt(position, direction, up);
        projection_matrix_ = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        
        is_inited_ = true;
        return 0;
    }

    int OpenGLCamera::Destroy() {
        return 0;
    }

    void OpenGLCamera::SetPosition(OpenGLCameraParams_t params) {
        params_ = params;

        glm::vec3 position = glm::vec3(params_.pos_x_, params_.pos_y_, params_.pos_z_);
        glm::vec3 direction = glm::vec3(params_.dir_x_, params_.dir_y_, params_.dir_z_);
        glm::vec3 up = glm::vec3(params_.up_x_, params_.up_y_, params_.up_z_);

        view_matrix_ = glm::lookAt(position, direction, up);
    }

    void OpenGLCamera::Move2D(float move_x, float move_y, float move_z) {
        glm::vec3 position = glm::vec3(params_.pos_x_ += move_x, params_.pos_y_ += move_y, params_.pos_z_ += move_z);
        glm::vec3 direction = glm::vec3(params_.dir_x_ += move_x, params_.dir_y_ += move_y, params_.dir_z_ += move_z);
        glm::vec3 up = glm::vec3(params_.up_x_, params_.up_y_, params_.up_z_);

        view_matrix_ = glm::lookAt(position, direction, up);
    }

    int OpenGLCamera::SetView() {
        if (!is_inited_) return false;

        glUniformMatrix4fv(shader_vars_.uni_View_, 1, GL_FALSE, &(view_matrix_[0][0]));
        glUniformMatrix4fv(shader_vars_.uni_Projection_, 1, GL_FALSE, &(projection_matrix_[0][0]));

        return 0;
    }

}