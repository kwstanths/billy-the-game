#ifndef __OpenGLFont_hpp__
#define __OpenGLFont_hpp__

#include <map>

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLShaders.hpp"

#include "glm/glm.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    /* Struct represeting a renderable character */
    typedef struct {
        /* Character texture */
        GLuint texture_id_;
        /* Size of the character */
        glm::ivec2 size_;
        /* Bearing of the character, offset from baseline to left/top of glyph */
        glm::ivec2 bearing_;
        /* Horizontal offset to advance to next character, the amount of space before and between for normal text */
        signed long advance_;
    } OpenGLCharacter_t;

    /* A class to draw text */
    class OpenGLText {
    public:
        OpenGLText();

        /** 
            Initialize the font using a .ttf file 
            @param font_file_path A file path of a .ttf file
            @param context opengl context class
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::string font_file_path, OpenGLContext * context);

        /**
            Clears the font initialised
            @return 0=OK
        */
        int Destroy();

        /**
            Return whether the font is initialised or not
            @return A boolean
        */
        bool IsInited();

        /**
            Draw a 2d text on a certain position on the screen, not in the world
            @param text The text to draw
            @param x The horizontal position in screen coordinates
            @param y The vertical position in screen coordintates
            @param scale The size of the text
            @param color The color of the text in RGB format
            @return 0=OK, -1=Font was not initialised
        */
        int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        bool is_inited_;
        std::map<GLchar, OpenGLCharacter_t> characters;

        GLuint VAO2DText_, VBO2DText_, VAO_Quad_;
        glm::mat4 text_projection_matrix_;
        OpenGLShaderText shader_text_;

        /**
            Get a renderable character object
        */
        OpenGLCharacter_t GetCharacter(GLchar character);

    };

}
}
}

#endif