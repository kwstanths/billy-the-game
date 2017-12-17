#include "OpenGLRenderer.hpp"

namespace game_engine {

    OpenGLRenderer::OpenGLRenderer() {

        is_inited_ = false;
    }

    int OpenGLRenderer::Init(OpenGLContext * context) {
        context_ = context;

        shader_vars_ = context_->GetShaderVariables();

        /* Configure a VAO */
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        is_inited_ = true;
        return 0;
    }

    int OpenGLRenderer::Destory() {

        is_inited_ = false;
        return 0;
    }

    bool OpenGLRenderer::IsInited() {
        return is_inited_;
    }

    int OpenGLRenderer::Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model) {
        if (!is_inited_) return -1;
        if (!object->IsInited()) return -1;
        if (!texture->IsInited()) return -1;

        /* */
        glUniformMatrix4fv(shader_vars_.uni_Model_, 1, GL_FALSE, &model[0][0]);

        /* */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());
        glUniform1i(shader_vars_.uni_Texture_, 0);

        /* Attribute number 0 is the cube model vertices */
        glBindBuffer(GL_ARRAY_BUFFER, object->GetVertexBufferID());
        glVertexAttribPointer(shader_vars_.attr_vertex_position_, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        /* Attribute number 1 is the cube's uv coordinates */
        glBindBuffer(GL_ARRAY_BUFFER, object->GetUVBufferID());
        glVertexAttribPointer(shader_vars_.attr_vertex_uv_, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, object->GetNoFTriangles());

        return 0;
    }

}