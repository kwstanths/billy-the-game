#ifndef __OpenGLTexture_hpp__
#define __OpenGLTexture_hpp__

#include "OpenGLIncludes.hpp"

#include <string>

namespace game_engine {

    class OpenGLTexture {
    public:
        enum texture_type {
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
        int Init(std::string file_path, texture_type type);

        /**
        
        */
        int Destory();

        /**
        
        */
        bool IsInited();

        /**
        
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
        
        */
        int LoadBMP(const char * imagepath, GLuint * texture_id);
    };
}

#endif