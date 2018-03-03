#ifndef __OpenGLObject_hpp__
#define __OpenGLObject_hpp__

#include <string>
#include <cstring>
#include <vector>

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
            Return whether the object is initialised
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
            Get the normal buffer OpenGL ID
            @return The id
        */
        GLuint GetNormalBufferID();

        /**
            Get the indexed elements buffer Opengl ID
            @return The id
        */
        GLuint GetElementBufferID();

        /**
            Get the number of triangles to draw
            @return The number of triangles
        */
        size_t GetNoFElements();

    private:
        /* A structure to represent the data for an OpenGL vertex */
        struct PackedVertex {
            glm::vec3 position_;
            glm::vec2 uv_;
            glm::vec3 normal_;
            bool operator<(const PackedVertex that) const {
                return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
            };
        };
        
        bool is_inited_;

        size_t total_indices_;
        GLuint vertex_buffer_, uv_buffer_, normal_buffer_, element_buffer_;

        /**
            
        */
        void IndexVBO(std::vector<glm::vec3> & in_vertices, std::vector<glm::vec2> & in_uvs, std::vector<glm::vec3> & in_normals,
            std::vector<unsigned short> & out_indices, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);

    };

}

#endif
