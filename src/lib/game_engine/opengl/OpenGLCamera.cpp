#include "OpenGLCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace game_engine {

    OpenGLCamera::OpenGLCamera() {
        is_inited_ = false;
    }

    int OpenGLCamera::Init(float pos_x, float pos_y, float pos_z,
        float dir_x, float dir_y, float dir_z,
        float up_x, float up_y, float up_z,
        OpenGLContext * context) 
    {
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;
        dir_x_ = dir_x;
        dir_y_ = dir_y;
        dir_z_ = dir_z;
        up_x_ = up_x;
        up_y_ = up_y;
        up_z_ = up_z;

        shader_vars_ = context->GetShaderVariables();

        glm::vec3 position = glm::vec3(pos_x_, pos_y_, pos_z_);
        glm::vec3 direction = glm::vec3(dir_x_, dir_y_, dir_z_);
        glm::vec3 up = glm::vec3(up_x_, up_y_, up_z_);
        
        view_matrix_ = glm::lookAt(position, direction, up);
        projection_matrix_ = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        
        is_inited_ = true;
        return 0;
    }

    int OpenGLCamera::Destroy() {
        return 0;
    }

    void OpenGLCamera::Fun(float test_x, float test_y, float test_z) {
        glm::vec3 position = glm::vec3(pos_x_ += test_x, pos_y_ += test_y, pos_z_ += test_z);
        glm::vec3 direction = glm::vec3(dir_x_, dir_y_, dir_z_);
        glm::vec3 up = glm::vec3(up_x_, up_y_, up_z_);

        view_matrix_ = glm::lookAt(position, direction, up);
    }

    int OpenGLCamera::SetView() {
        if (!is_inited_) return false;

        glUniformMatrix4fv(shader_vars_.uni_View_, 1, GL_FALSE, &(view_matrix_[0][0]));
        glUniformMatrix4fv(shader_vars_.uni_Projection_, 1, GL_FALSE, &(projection_matrix_[0][0]));

        return 0;
    }

}