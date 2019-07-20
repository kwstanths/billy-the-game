#include "OpenGLText.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp>

#include "game_engine/ErrorCodes.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLText::OpenGLText() {
        is_inited_ = false;
    }

    int OpenGLText::Init(std::string font_file_path, OpenGLContext * context) {
        if (is_inited_) return -1;

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            return Error::ERROR_FREETYPE_LIB;

        /* Load font */
        FT_Face face;
        if (FT_New_Face(ft, font_file_path.c_str() , 0, &face))
            return Error::ERROR_FREETYPE_FONT;

        FT_Set_Pixel_Sizes(face, 0, 48);

        /* Disable byte-alignment restriction */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Load first 128 characters of ASCII set */
        for (GLubyte c = 0; c < 128; c++) {
            /* Load character glyph  */
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                PrintError(Error::ERROR_FREETYPE_GLYPH);
                continue;
            }
            /* Generate character texture */
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            /* Set texture options */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            OpenGLCharacter_t character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            characters.insert(std::pair<GLchar, OpenGLCharacter_t>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        shader_text_ = context->shader_text_;

        glGenVertexArrays(1, &VAO2DText_);
        glBindVertexArray(VAO2DText_);
        glGenBuffers(1, &VBO2DText_);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2DText_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        text_projection_matrix_ = glm::ortho(0.0f, static_cast<GLfloat>(context->GetWindowWidth()), 0.0f, static_cast<GLfloat>(context->GetWindowHeight()));

        is_inited_ = true;
        return 0;
    }

    int OpenGLText::Destroy() {
        characters.clear();
        is_inited_ = false;
        return 0;
    }

    bool OpenGLText::IsInited() {
        return is_inited_;
    }

    OpenGLCharacter_t OpenGLText::GetCharacter(GLchar character) {
        
        if (!is_inited_) {
            debug_tools::Console(debug_tools::FATAL, "OpenGLFont::GetCharacter() - Object not initialised");
            return OpenGLCharacter_t();
        }

        return characters[character];
    }

    int OpenGLText::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {

        if (!is_inited_) return -1;

        glDisable(GL_DEPTH_TEST);

        shader_text_.Use();
        glBindVertexArray(VAO2DText_);

        glUniform3f(shader_text_.uni_Texture_color_, color.x, color.y, color.z);
        glUniformMatrix4fv(shader_text_.uni_Projection_, 1, GL_FALSE, &(text_projection_matrix_[0][0]));
        glActiveTexture(GL_TEXTURE0);

        /* Iterate through the text characters */
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            OpenGLCharacter_t ch = GetCharacter(*c);

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

        glEnable(GL_DEPTH_TEST);

        return 0;
    }


}
}
}