#include "OpenGLRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/graphics/Mesh.hpp"
#include "game_engine/math/HelpFunctions.hpp"

#include "game_engine/graphics/AssetManager.hpp"

#include "debug_tools/Console.hpp"

namespace math = game_engine::math;


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
        shader_main_ = context_->shader_main_;
        shader_model_texture_ = context_->shader_model_texture_;
        shader_vertices_color_ = context_->shader_vertices_color_;
        shader_text_ = context_->shader_text_;

        /* We mainly draw with the main shader */
        shader_main_.Use();
        /* Set the texture IDs on the 2D samplers used */
        shader_main_.SetUniformInt(shader_main_.GetUniformLocation("object_material.texture_diffuse"), 0);
        shader_main_.SetUniformInt(shader_main_.GetUniformLocation("object_material.texture_specular"), 1);

        /* Configure simple shader as well */
        shader_model_texture_.Use();
        /* Set the texture ID on the 2D sampler used */
        shader_model_texture_.SetUniformInt(shader_model_texture_.uni_texture_, 0);

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
        /* TODO Destroy Text rendering stuff */
        is_inited_ = false;
        return 0;
    }

    bool OpenGLRenderer::IsInited() {
        return is_inited_;
    }

    void OpenGLRenderer::SetView(OpenGLCamera * camera) {

        camera->CalculateView();

        shader_main_.Use();
        shader_main_.SetUniformMat4(shader_main_.uni_View_, camera->view_matrix_);
        shader_main_.SetUniformMat4(shader_main_.uni_Projection_, camera->projection_matrix_);
        shader_main_.SetUniformVec3(shader_main_.uni_camera_position_worldspace_, camera->config_.position_);

        shader_model_texture_.Use();
        shader_model_texture_.SetUniformMat4(shader_model_texture_.uni_View_, camera->view_matrix_);
        shader_model_texture_.SetUniformMat4(shader_model_texture_.uni_Projection_, camera->projection_matrix_);

        shader_vertices_color_.Use();
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_View_, camera->view_matrix_);
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Projection_, camera->projection_matrix_);
    }

    int OpenGLRenderer::Draw(OpenGLObject & object, std::vector<OpenGLTexture *> & textures, glm::mat4 model, Material_t mtl) {

        /* TODO Check input arguments */

        shader_main_.Use();
        /* Set object material */
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.ambient"), mtl.ambient_);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.diffuse"), mtl.diffuse_);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.specular"), mtl.specular_);
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("object_material.shininess"), mtl.shininess_);
        /* Set the model uniform */
        shader_main_.SetUniformMat4(shader_main_.uni_Model_, model);

        glBindVertexArray(object.VAO_);

        object.SetupAttributes(&shader_main_);

        for (size_t i = 0; i < textures.size(); i++) {
            textures[i]->ActivateTexture(i);
        }
        
        /* Draw with index buffer */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.GetElementBufferID());
        glDrawElements(GL_TRIANGLES, object.GetNoFElements(), GL_UNSIGNED_INT, (void*)0);

        /* Unbind everything */
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        return 0;
    }

    int OpenGLRenderer::Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model) {

        if (!is_inited_) return -1;
        if (!object->IsInited()) return -1;
        if (!texture->IsInited()) return -1;

        shader_model_texture_.Use();
        /* Set the model uniform */
        shader_model_texture_.SetUniformMat4(shader_model_texture_.uni_Model_, model);

        glBindVertexArray(object->VAO_);

        object->SetupAttributes(&shader_model_texture_);

        texture->ActivateTexture(0);

        /* Draw with index buffer */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->GetElementBufferID());
        glDrawElements(GL_TRIANGLES, object->GetNoFElements(), GL_UNSIGNED_INT, (void*)0);

        /* Unbind */
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shader_main_.Use();

        return 0;
    }

    int OpenGLRenderer::DrawLineXY(float start_x, float start_y, float stop_x, float stop_y, float z_height, float size, glm::vec3 color) {

        if (!is_inited_) return -1;

        /* Find the angle of the line, and the offsets of the four points that will create the rectangle - line */
        float line_angle = atan((stop_y - start_y) / (stop_x - start_x));
        float horizontal_offset = size * cos(math::GetRadians(90) + line_angle);
        float vertical_offset = size * sin(math::GetRadians(90) + line_angle);
        /* Create the four points, 1-2-3-4 are in circular order */
        float point_1_x = start_x + horizontal_offset;
        float point_1_y = start_y + vertical_offset;
        float point_2_x = start_x - horizontal_offset;
        float point_2_y = start_y - vertical_offset;
        float point_3_x = stop_x - horizontal_offset;
        float point_3_y = stop_y - vertical_offset;
        float point_4_x = stop_x + horizontal_offset;
        float point_4_y = stop_y + vertical_offset;

        /* Create the vertices, uvs and normal pay no role at all */
        std::vector<Vertex_t> line_vertices(4);
        line_vertices.at(0).position_ = { point_1_x, point_1_y, z_height };
        line_vertices.at(0).uv_ = { 0,1 };
        line_vertices.at(0).normal_ = { 0,0,1 };
        line_vertices.at(1).position_ = { point_2_x, point_2_y, z_height };
        line_vertices.at(1).uv_ = { 0,0 };
        line_vertices.at(1).normal_ = { 0,0,1 };
        line_vertices.at(2).position_ = { point_3_x, point_3_y, z_height };
        line_vertices.at(2).uv_ = { 1,0 };
        line_vertices.at(2).normal_ = { 0,0,1 };
        line_vertices.at(3).position_ = { point_4_x, point_4_y, z_height };
        line_vertices.at(3).uv_ = { 1,1 };
        line_vertices.at(3).normal_ = { 0,0,1 };
        /* Set the indices for the 2 triangles to draw */
        std::vector<unsigned int> line_indices(6);
        line_indices.at(0) = 0;
        line_indices.at(1) = 1;
        line_indices.at(2) = 2;
        line_indices.at(3) = 0;  
        line_indices.at(4) = 2;
        line_indices.at(5) = 3;

        /* Create an OpenGLObejct */
        OpenGLObject line;
        line.Init(line_vertices, line_indices);

        /* Set the simplest shader, and set the color */
        shader_vertices_color_.Use();
        shader_vertices_color_.SetUniformVec3(shader_vertices_color_.GetUniformLocation("fragment_color"), color);

        /* Set the OpenGLObject attributes to the shader */
        glBindVertexArray(line.VAO_);
        line.SetupAttributes(&shader_vertices_color_);
        
        /* Draw with index buffer */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line.GetElementBufferID());
        
        /** 
            Disable face culling for the drawing so that our line is visible no matter the starting and stopping point
            Otherwise, the indexing of our triangles whould be relatively hard to implement
        */
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, line.GetNoFElements(), GL_UNSIGNED_INT, (void*)0);
        glEnable(GL_CULL_FACE);

        /* Unbind */
        glBindVertexArray(0);
        shader_main_.Use();

        line.Destroy();

        return 0;
    }

    int OpenGLRenderer::SetPointLightsNumber(unsigned int number) {
        
        shader_main_.Use();
        shader_main_.SetUniformUInt(shader_main_.GetUniformLocation("number_of_point_lights"), number);
        
        return 0;
    }

    int OpenGLRenderer::SetPointLight(std::string index, glm::vec3 position, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular,
        float attenuation_constant, float attenuation_linear, float attenuation_quadratic) 
    {
        std::string shader_name = "point_light[" + index + "]";

        shader_main_.Use();
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".position"), position);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".ambient"), color_ambient);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".diffuse"), color_diffuse);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".specular"), color_specular);

        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation(shader_name + ".constant"), attenuation_constant);
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation(shader_name + ".linear"), attenuation_linear);
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation(shader_name + ".quadratic"), attenuation_quadratic);

        return 0;
    }

    int OpenGLRenderer::SetDirectionalLight(glm::vec3 direction, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular) {

        shader_main_.Use();
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("global_illumination.direction"), direction);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("global_illumination.ambient"), color_ambient);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("global_illumination.diffuse"), color_diffuse);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("global_illumination.specular"), color_specular);

        return 0;
    }

    int OpenGLRenderer::SetSpotLight(glm::vec3 position, glm::vec3 direction, float inner_radius, float outer_radius,
        glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular, 
        float attenuation_constant, float attenuation_linear, float attenuation_quadratic)
    {
        shader_main_.Use();
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("cast_light.position"), position);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("cast_light.direction"), direction);
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("cast_light.inner_radius_cosine"), cos(inner_radius));
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("cast_light.outer_radius_cosine"), cos(outer_radius));

        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("cast_light.ambient"), color_ambient);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("cast_light.diffuse"), color_diffuse);
        shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("cast_light.specular"), color_specular);

        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("cast_light.constant"), attenuation_constant);
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("cast_light.linear"), attenuation_linear);
        shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("cast_light.quadratic"), attenuation_quadratic);

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