#include "OpenGLObject.hpp"

#include <vector>

#include "OpenGLMath.hpp"
#include "OpenGLLoadTexture.hpp"

#include "../ObjLoad.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine{

    OpenGLObject::OpenGLObject() {        
        is_inited_ = false;
    }

    int OpenGLObject::Init(OpenGLObjectConfig_t config, OpenGLContext * context) {
        if (is_inited_) return -1;

        SetPosition(config.pos_x_, config.pos_y_, config.pos_z_);
        
        shader_vars_ = context->GetShaderVariables();

        int ret = OpenGLLoadDDS(config.texture_path_.c_str(), &texture_);
        if (ret != 0) return ret;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        ret = LoadObj(config.object_path_.c_str(), vertices, uvs, normals);
        if (ret != 0) return ret;

        triangles_ = vertices.size();
        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &uv_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        /* Configure a VAO */
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);
        /* Attribute number 0 is the cube model vertices */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glVertexAttribPointer(shader_vars_.attr_vertex_position_, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        /* Attribute number 1 is the cube's uv coordinates */
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
        glVertexAttribPointer(shader_vars_.attr_vertex_uv_, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        is_inited_ = true;
        return 0;
    }

    int OpenGLObject::Destroy() {
        if (!is_inited_) return -1;

        glDeleteTextures(1, &texture_);
        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &uv_buffer_);
        glDeleteVertexArrays(1, &VAO_);

        is_inited_ = false;
        return 0;
    }

    int OpenGLObject::Draw(double delta_time) {
        if (!is_inited_) return -1;
        
        
        glBindVertexArray(VAO_);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);
        glUniform1i(shader_vars_.uni_Texture_, 0);

        glUniformMatrix4fv(shader_vars_.uni_Model_, 1, GL_FALSE, &model_[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, triangles_);

        return 0;
    }

    bool OpenGLObject::IsInited() {
        return is_inited_;
    }

    void OpenGLObject::SetPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z) {
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;
        model_ = GetTranslateMatrix(glm::vec3(pos_x, pos_y, pos_z));
    }

}
