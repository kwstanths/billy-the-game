#ifndef __OpenGLLoadTexture_hpp__
#define __OpenGLLoadTexture_hpp__

#include "OpenGLIncludes.hpp"

namespace game_engine {

    /**
        Load a DDS texture and get an OpenGL texture id 
    */
    int OpenGLLoadDDS(const char * imagepath, GLuint * texture_id);
}


#endif