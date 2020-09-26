#ifndef __OpenGLCubemap_hpp__
#define __OpenGLCubemap_hpp__

#include <string>
#include <vector>

#include "OpenGLIncludes.hpp"

namespace game_engine { namespace graphics { namespace opengl {

    class OpenGLCubemap {
    public:
    
        /**
            Does nothing
        */
        OpenGLCubemap();
    
        /**
            Reads the texture from the disk, and initializes an OpenGL texture
            @param file_path The path to the disk file
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::vector<std::string>& faces);
    
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
    
        /**
            Activate texture at position texture_id
            @param texture_id The position to activate the texture
        */
        void ActivateTexture(int texture_id);
    
    private:
        bool is_inited_;
        GLuint texture_;
        
        /**
            Load a BMP, JPG, PNG image and others, see proper documentation here https://github.com/nothings/stb/blob/master/stb_image.h
        */
        int LoadSTB(std::vector<std::string>& faces, GLuint * texture_id);
    
    };

} } }

#endif