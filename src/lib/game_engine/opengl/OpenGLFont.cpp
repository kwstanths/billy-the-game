#include "OpenGLFont.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "../ErrorCodes.hpp"

namespace game_engine {

    OpenGLFont::OpenGLFont() {
        is_inited_ = false;
    }

    int OpenGLFont::Init(std::string font_file_path) {
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

        is_inited_ = true;
        return 0;
    }

    int OpenGLFont::Destroy() {
        characters.clear();
        is_inited_ = false;
        return 0;
    }

    bool OpenGLFont::IsInited() {
        return is_inited_;
    }

    OpenGLCharacter_t OpenGLFont::GetCharacter(GLchar character) {
        
        if (!is_inited_) {
            debug_tools::Console(debug_tools::FATAL, "OpenGLFont::GetCharacter() - Object not initialised");
            return OpenGLCharacter_t();
        }

        return characters[character];
    }


}