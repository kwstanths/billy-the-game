#ifndef __OpenGLTexture_hpp__
#define __OpenGLTexture_hpp__

#include "OpenGLIncludes.hpp"

#include <string>

namespace game_engine {

    class OpenGLTexture {
    public:
        enum OpenGLTextureType {
            TEXTURE_BMP,
            TEXTURE_DDS,
        };

        /**
            Does nothing
        */
        OpenGLTexture();

        /**
            Reads the texture from the disk, and initializes an OpenGL texture
            @param file_path The path to the disk file
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::string file_path, OpenGLTextureType type);

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

    private:
        bool is_inited_;

        GLuint texture_;

        /**
            Load a DDS texture and get an OpenGL texture id 
        */
        int LoadDDS(const char * imagepath, GLuint * texture_id);

        /**
            Load a BMP texture and get an OpenGL texture id
        */
        int LoadBMP(const char * imagepath, GLuint * texture_id);
    };
}

#endif