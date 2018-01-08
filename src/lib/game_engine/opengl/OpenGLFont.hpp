#ifndef __OpenGLFont_hpp__
#define __OpenGLFont_hpp__

#include <map>

#include "glm/glm.hpp"

#include "OpenGLIncludes.hpp"

namespace game_engine {

    /* Struct represeting a renderable character */
    typedef struct {
        /* Character texture */
        GLuint texture_id_;
        /* Size of the character */
        glm::ivec2 size_;
        /* Bearing of the character, offset from baseline to left/top of glyph */
        glm::ivec2 bearing_;
        /* Horizontal offset to advance to next character, the amount of space before and between for normal text */
        GLuint advance_;
    } OpenGLCharacter_t;


    class OpenGLFont {
    public:
        OpenGLFont();

        /** 
            Initialize the font using a .ttf file 
            @param font_file_path A file path of a .ttf file
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::string font_file_path);

        /**
            Clears the font initialised
            @return 0=OK
        */
        int Destroy();

        /**
            Return wether the font is initialised or not
            @return A boolean
        */
        bool IsInited();

        /**
            Get a renderable character object
        */
        OpenGLCharacter_t GetCharacter(GLchar character);

    private:
        bool is_inited_;
        std::map<GLchar, OpenGLCharacter_t> characters;
        GLuint VAO, VBO;
    };

}

#endif