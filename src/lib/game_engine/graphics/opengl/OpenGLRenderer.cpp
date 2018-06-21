#include "OpenGLRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "game_engine/graphics/Material.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLRenderer::OpenGLRenderer() {

        is_inited_ = false;
        font_ = new OpenGLFont();
    }

    int OpenGLRenderer::Init(OpenGLContext * context) {
        context_ = context;

        /* Initialize font */
        std::string font_location = context->GetFontLocation();
        if (font_->Init(font_location) != 0)
            debug_tools::ConsoleInfoL(debug_tools::CRITICAL, "Font not found", "name", font_location);

        /* Get shader variables */
        shader_main_ = context_->GetShaderVariables();
        shader_text_ = context_->GetShaderTextVariables();

        /* Configure a VAO for main shader */
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        /* Configure VAO/VBO for text shader */
        glGenVertexArrays(1, &VAO2DText_);
        glBindVertexArray(VAO2DText_);
        glGenBuffers(1, &VBO2DText_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2DText_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        text_projection_matrix_ = glm::ortho(0.0f, static_cast<GLfloat>(context_->GetWindowWidth()), 0.0f, static_cast<GLfloat>(context_->GetWindowHeight()));

        is_inited_ = true;
        return 0;
    }

    int OpenGLRenderer::Destroy() {

        is_inited_ = false;
        return 0;
    }

    bool OpenGLRenderer::IsInited() {
        return is_inited_;
    }

    int OpenGLRenderer::Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model, Material_t mtl) {
        if (!is_inited_) return -1;
        if (!object->IsInited()) return -1;
        if (!texture->IsInited()) return -1;

        glBindVertexArray(VAO_);

        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.ambient"), mtl.ambient_);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.diffuse"), mtl.diffuse_);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.specular"), mtl.specular_);
        shader_main_.SetUniFloat(shader_main_.GetUniformLocation("object_material.shininess"), mtl.shininess_);

        /* Set the model uniform */
        shader_main_.SetUniformMat4(shader_main_.uni_Model_, model);

        /* Set the texture uniform */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());
        glUniform1i(shader_main_.uni_Texture_, 0);

        /* Attribute number 0 is the object vertices */
        glBindBuffer(GL_ARRAY_BUFFER, object->GetVertexBufferID());
        glVertexAttribPointer(shader_main_.attr_vertex_position_, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        /* Attribute number 1 is the object's uv coordinates */
        glBindBuffer(GL_ARRAY_BUFFER, object->GetUVBufferID());
        glVertexAttribPointer(shader_main_.attr_vertex_uv_, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        /* Attribute number 2 is the object's normals */
        glBindBuffer(GL_ARRAY_BUFFER, object->GetNormalBufferID());
        glVertexAttribPointer(shader_main_.attr_vertex_normal_, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        /* Draw with index buffer */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->GetElementBufferID());
        glDrawElements(GL_TRIANGLES, object->GetNoFElements(), GL_UNSIGNED_SHORT, (void*)0);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        return 0;
    }

    int OpenGLRenderer::SetLight(glm::vec3 position, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular) {

        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("light_properties.position"), position);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("light_properties.ambient"), color_ambient);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("light_properties.diffuse"), color_diffuse);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("light_properties.specular"), color_specular);

        return 0;
    }

    int OpenGLRenderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
        if (!font_->IsInited()) return -1;

        shader_text_.Use();
        glBindVertexArray(VAO2DText_);

        glUniform3f(shader_text_.uni_Texture_color_, color.x, color.y, color.z);
        glUniformMatrix4fv(shader_text_.uni_Projection_, 1, GL_FALSE, &(text_projection_matrix_[0][0]));
        glActiveTexture(GL_TEXTURE0);
        
        /* Iterate through the text characters */
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            OpenGLCharacter_t ch = font_->GetCharacter(*c);

            GLfloat xpos = x + ch.bearing_.x * scale;
            GLfloat ypos = y - (ch.size_.y - ch.bearing_.y) * scale;

            GLfloat w = ch.size_.x * scale;
            GLfloat h = ch.size_.y * scale;
            /* Update text VBO for each character */
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };
            glBindTexture(GL_TEXTURE_2D, ch.texture_id_);
            /* Update text VBO */
            glBindBuffer(GL_ARRAY_BUFFER, VBO2DText_);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            /* Render text */
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisable(GL_BLEND);

            /* Now advance cursors for next glyph (note that advance is number of 1/64 pixels) */
            /* Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels)) */
            x += (ch.advance_ >> 6) * scale;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        shader_main_.Use();

        return 0;
    }

}
}
}