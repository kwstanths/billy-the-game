#ifndef __OpenGLObject_hpp__
#define __OpenGLObject_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

#include <glm/glm.hpp>

namespace game_engine {

    class OpenGLObject {
    public:
        /**
            Does nothing
        */
        OpenGLObject();

        /**
            
        */
        int Init(std::string object_path);

        /**
            
        */
        int Destroy();

        /**
            Return wether the object is initialised
        */
        bool IsInited();

        /**
        
        */
        GLuint GetVertexBufferID();

        /**
        
        */
        GLuint GetUVBufferID();

        /**
        
        */
        size_t GetNoFTriangles();

    private:
        bool is_inited_;

        size_t triangles_;
        GLuint vertex_buffer_, uv_buffer_;

    };

}

#endif