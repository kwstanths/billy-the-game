#ifndef __OpenGLObject_hpp__
#define __OpenGLObject_hpp__

#include <string>
#include <cstring>
#include <vector>

#include "game_engine/graphics/GraphicsTypes.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLShaders.hpp"

#include <glm/glm.hpp>

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLObject {
        friend class OpenGLRenderer;
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
        int Init(std::vector<game_engine::graphics::Vertex_t> & vertices, std::vector<unsigned int> & indices);

        void SetupAttributes(OpenGLShaderMain * shader);

        void SetupAttributes(OpenGLShaderSimple * shader);

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
        bool is_inited_;

        size_t total_indices_;
        GLuint VAO_, vertex_buffer_, element_buffer_;

        
    };

}
}
}

#endif
