#ifndef __OpenGLTexture_hpp__
#define __OpenGLTexture_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLTexture {
    public:

        /**
            Does nothing
        */
        OpenGLTexture();

        /**
            Reads the texture from the disk, and initializes an OpenGL texture
            @param file_path The path to the disk file
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::string file_path, int type);

        /**
            Deletes allocated objects, needs Init to be called again. Never fails
            @return 0 = OK
        */
        int Destroy();

        /**
            Return whether the object is initialised
            @return true = Initialised, false = not Initialised
        */
        bool IsInited();

        /**
            Get the opengl tecture id
            @return The ID
        */
        GLuint GetID();

        void ActivateTexture(int texture_id);

    private:
        bool is_inited_;
        int type_;
        GLuint texture_;

        /**
            Load a DDS texture and get an OpenGL texture id 
        */
        int LoadDDS(const char * imagepath, GLuint * texture_id);

        /**
            Load a BMP, JPG, PNG image and others, see proper documentation here https://github.com/nothings/stb/blob/master/stb_image.h
        */
        int LoadSTB(const char * imagepath, GLuint * texture_id);

    };
}
}
}

#endif