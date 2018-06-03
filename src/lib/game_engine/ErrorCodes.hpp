#ifndef __ErrorCodes_hpp__
#define __ErrorCodes_hpp__

#include "debug_tools/Console.hpp"

namespace game_engine {
    
    enum Error {
        ERROR_GEN_NOT_INIT = -1,
        ERROR_NO_ERROR,

        ERROR_GLFW_INIT,
        ERROR_GLFW_WINDOW,
        ERROR_GLEW_INIT,

        ERROR_SHADER_FILES_NOT_FOUND,
        ERROR_SHADER_RES_NOT_FOUND,
        ERROR_SHADER_COMPILE,

        ERROR_ENGINE_NOT_INIT,
        ERROR_WORLD_NOT_INIT,

        ERROR_ASSET_NOT_FOUND,
        ERROR_ASSET_NOT_DDS,
        ERROR_ASSET_DDS_COMPRESSION,
        ERROR_ASSET_BMP_ERROR,

        ERROR_OBJECT_PARSE,
        ERROR_OBJECT_NOT_INIT,
        ERROR_TEXTURE_NOT_INIT,

        ERROR_FREETYPE_LIB,
        ERROR_FREETYPE_FONT,
        ERROR_FREETYPE_GLYPH,

        ERROR_PHYSICS_INIT,
    };

    void PrintError(int error);

}


#endif