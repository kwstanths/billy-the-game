#include "OpenGLObject.hpp"

#include "OpenGLMath.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine{

    OpenGLObject::OpenGLObject(OpenGLObjectParams_t params) {
        params_ = params;
        
        is_inited_ = false;
    }

    int OpenGLObject::Init(OpenGLContext * context) 
    {
        if (is_inited_) return -1;

        SetPosition(params_.pos_x_, params_.pos_y_, params_.pos_z_);
        
        shader_vars_ = context->GetShaderVariables();

        /* Vertex positions */
        static const GLfloat vertex_buffer_data[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
        };


        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        /* Vertex colors */
        static const GLfloat color_buffer_data[] = {
            1.0f,  0.0f,  0.0f,
            0.0f,  0.0f,  0.0f,
            1.0f,  1.0f,  1.0f,
            0.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  1.0f,  1.0f,
        };

        glGenBuffers(1, &color_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

        /* Configure a VAO */
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        /* Attribute number 0 is the cube model vertices */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glVertexAttribPointer(shader_vars_.attr_vertex_position_, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        /* Attribute number 1 is the cube's uv coordinates */
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glVertexAttribPointer(shader_vars_.attr_vertex_color_, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);


        is_inited_ = true;
        return 0;
    }

    int OpenGLObject::Destroy() {
        if (!is_inited_) return -1;

        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &color_buffer_);
        glDeleteVertexArrays(1, &VAO);

        is_inited_ = false;
        return 0;
    }

    int OpenGLObject::Draw(double delta_time) {
        if (!is_inited_) return -1;
        
        glBindVertexArray(VAO);
        glUniformMatrix4fv(shader_vars_.uni_Model_, 1, GL_FALSE, &model_[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

        return 0;
    }

    void OpenGLObject::SetPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z) {
        params_.pos_x_ = pos_x;
        params_.pos_y_ = pos_y;
        params_.pos_z_ = pos_z;
        model_ = GetTranslateMatrix(glm::vec3(pos_x, pos_y, pos_z));
    }

}
