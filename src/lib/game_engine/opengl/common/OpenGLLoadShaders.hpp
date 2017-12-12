#ifndef __OpenGLLoadShaders_hpp__
#define __OpenGLLoadShaders_hpp__

#include "OpenGLIncludes.hpp"

namespace game_engine {

    int OpenGLLoadShaders(const char * vertex_file_path,const char * fragment_file_path, GLuint * program_id);

}


#endif
