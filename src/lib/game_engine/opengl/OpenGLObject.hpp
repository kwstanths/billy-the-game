#ifndef __OpenGLObject_hpp__
#define __OpenGLObject_cpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLShaderVariables.hpp"
#include "OpenGLContext.hpp"

#include <glm/glm.hpp>

namespace game_engine {

    class OpenGLObject {
    public:
        OpenGLObject();

        int Init(const char * object_path, 
            const char * texture_path, 
            int pos_x, int pos_y, int pos_z, 
            OpenGLContext * context_);

        int Destroy();

        int Draw(double delta_time);

    private:
        bool is_inited_;
        glm::mat4 model_;
        OpenGLShaderVariables_t shader_vars_;

        GLuint VAO, vertex_buffer_, color_buffer_;
    };

}

#endif