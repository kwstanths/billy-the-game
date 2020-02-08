#include "ErrorCodes.hpp"

namespace game_engine {

    void PrintError(int error) {
        debug_tools::Level level;
        std::string text;
        switch (error)
        {
        case ERROR_GEN_NOT_INIT:
            level = debug_tools::CRITICAL;
            text = "Object is not initialised";
            break;
        case ERROR_NO_ERROR:
            level = debug_tools::INFO;
            text = "No error";
            break;
        case ERROR_GLFW_INIT:
            level = debug_tools::FATAL;
            text = "Cannot initialize GLFW";
            break;
        case ERROR_GLFW_WINDOW:
            level = debug_tools::FATAL;
            text = "Cannot open GLFW window";
            break;
        case ERROR_GLEW_INIT:
            level = debug_tools::FATAL;
            text = "Cannot initialize GLEW";
            break;
        case ERROR_SHADER_FILES_NOT_FOUND:
            level = debug_tools::FATAL;
            text = "Cannot find shader files";
            break;
        case ERROR_SHADER_RES_NOT_FOUND:
            level = debug_tools::FATAL;
            text = "Cannot find shader variables";
            break;
        case ERROR_SHADER_COMPILE:
            level = debug_tools::FATAL;
            text = "Shader compilation error";
            break;
        case ERROR_ENGINE_NOT_INIT:
            level = debug_tools::FATAL;
            text = "Engine is not initialised";
            break;
        case ERROR_WORLD_NOT_INIT:
            level = debug_tools::FATAL;
            text = "World sector is not initialised";
            break;
        case ERROR_ASSET_NOT_FOUND:
            level = debug_tools::CRITICAL;
            text = "Asset not found";
            break;
        case ERROR_ASSET_NOT_DDS:
            level = debug_tools::CRITICAL;
            text = "Asset is not DDS file";
            break;
        case ERROR_ASSET_DDS_COMPRESSION:
            level = debug_tools::CRITICAL;
            text = "DDS asset compression error";
            break;
        case ERROR_ASSET_BMP_ERROR:
            level = debug_tools::CRITICAL;
            text = "BMP asset error";
            break;
        case ERROR_OBJECT_PARSE:
            level = debug_tools::CRITICAL;
            text = "Object parsing error";
            break;
        case ERROR_OBJECT_NOT_INIT:
            level = debug_tools::CRITICAL;
            text = "Object is not initialised";
            break;
        case ERROR_TEXTURE_NOT_INIT:
            text = "Texture is not initialised";
            level = debug_tools::CRITICAL;
            break;
        case ERROR_FREETYPE_LIB:
            text = "Could not find FreeType library";
            level = debug_tools::CRITICAL;
            break;
        case ERROR_FREETYPE_FONT:
            text = "Error loading font";
            level = debug_tools::CRITICAL;
            break;
        case ERROR_FREETYPE_GLYPH:
            text = "Error loading font character";
            level = debug_tools::CRITICAL;
            break;
        case ERROR_OUT_OF_REGION:
            text = "Object is out of region";
            level = debug_tools::WARNING;
            break;
        default:
            text = "Unkown error";
            level = debug_tools::WARNING;
            break;
        }

        debug_tools::Console(level, text);
    }

}