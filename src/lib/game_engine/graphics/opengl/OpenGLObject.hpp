#ifndef __OpenGLObject_hpp__
#define __OpenGLObject_hpp__

#include <string>
#include <cstring>
#include <vector>

#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/math/Types.hpp"

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
        int Init(std::vector<game_engine::graphics::Vertex_t> & vertices, std::vector<unsigned int> & indices, bool generate_bbox_info = true);

        void SetupAttributes(OpenGLShaderMain * shader);

        void SetupAttributes(OpenGLShaderModelTexture * shader);

        void SetupAttributes(OpenGLShaderVerticesColor * shader);

        void SetupAttributes(OpenGLShaderGBuffer * shader);

        void Render();

        void RenderBoundingBoxLines();

        void RenderBoundingBoxFaces();

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

        game_engine::Real_t GetBoundingBoxVolume();
        /* Bounding box values */
        GLfloat min_x_, max_x_, min_y_, max_y_, min_z_, max_z_;

    private:        
        bool is_inited_;

        size_t total_indices_;
        GLuint VAO_, vertex_buffer_, element_buffer_;

        /* Bounding box values */
        size_t total_indices_bbox_, total_indices_bbox_faces_;
        GLuint VAO_bbox_, VAO_bbox_faces_, vertex_buffer_bbox_, element_buffer_bbox_, element_buffer_bbox_faces_;
        glm::mat4 bbox_transform_;

        void GenerateBoundingBox(std::vector<game_engine::graphics::Vertex_t>& vertices);
    };

}
}
}

#endif
