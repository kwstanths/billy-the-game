#ifndef __OpenGLCamera_hpp__
#define __OpenGLCamera_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLShaderVariables.hpp"
#include "OpenGLContext.hpp"

#include <glm/glm.hpp>

namespace game_engine {

    class OpenGLCamera {
    public:
        OpenGLCamera();

        int Init(float pos_x, float pos_y, float pos_z, 
            float dir_x, float dir_y, float dir_z,
            float up_x, float up_y, float up_z,
            OpenGLContext * context);

        int Destroy();

        void Fun(float test_x, float test_y, float test_z);

        int SetView();

    private:
        bool is_inited_;
        OpenGLShaderVariables_t shader_vars_;

        float pos_x_, pos_y_, pos_z_;
        float dir_x_, dir_y_, dir_z_;
        float up_x_, up_y_, up_z_;

        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;
    };
}

#endif