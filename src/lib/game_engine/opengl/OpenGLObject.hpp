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
            Reads the object_path for a .obj file, and initializes the OpenGL stuff
            @param The disk file path of the .obj file
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::string object_path);

        /**
            Deallocates the OpenGL stuf initialized
            @return 0=OK, -1=Not initialized
        */
        int Destroy();

        /**
            Return wether the object is initialised
        */
        bool IsInited();

        /**
            Get the vertex buffer OpenGL ID
            @return The id
        */
        GLuint GetVertexBufferID();

        /**
            Get the UV buffer OpenGL ID
            @return The id
        */
        GLuint GetUVBufferID();

        /**
            Get the number of triangles to draw
            @return The number of triangles
        */
        size_t GetNoFTriangles();

    private:
        bool is_inited_;

        size_t triangles_;
        GLuint vertex_buffer_, uv_buffer_;

    };

}

#endif