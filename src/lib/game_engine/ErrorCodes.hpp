#ifndef __ErrorCodes_hpp__
#define __ErrorCodes_hpp__

namespace game_engine {
    
    enum Error {
        ERROR_GLFW_INIT = 1,
        ERROR_GLFW_WINDOW,
        ERROR_GLEW_INIT,
        ERROR_SHADER_FILES_NOT_FOUND,
        ERROR_SHADER_RES_NOT_FOUND,
        ERROR_SHADER_COMPILE,
        ERROR_ASSET_NOT_FOUND,
        ERROR_ASSET_NOT_DDS,
        ERROR_ASSET_DDS,
    };

}


#endif