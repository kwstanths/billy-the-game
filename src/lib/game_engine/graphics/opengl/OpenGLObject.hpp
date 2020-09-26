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
            Initialize OpenGL VAO and VBO
            @param vertices Object vertices, uvs, normals
            @param vertices Triangle infices
            @param generate_bbox_info Calculate bounding box?
            @return 0=OK, -1=Already initialised, else see ErrorCodes.hpp
        */
        int Init(std::vector<game_engine::graphics::Vertex_t> & vertices, std::vector<unsigned int> & indices, bool generate_bbox_info = true);

        void SetVertices(std::vector<game_engine::graphics::Vertex_t> & vertices);

        /* Setup shaders parameters */
        void SetupAttributes(OpenGLShaderVerticesColor * shader);
        void SetupAttributes(OpenGLShaderGBuffer * shader);
        void SetupAttributes(OpenGLShaderShadowMap * shader);
        void SetupAttributes(OpenGLShaderDrawNormals * shader);
        void SetupAttributes(OpenGLShaderDisplacement * shader);

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

    protected:
        bool is_inited_;

    private:        

        size_t total_indices_;
        GLuint VAO_, vertex_buffer_, element_buffer_;

        /* Bounding box values */
        size_t total_indices_bbox_, total_indices_bbox_faces_;
        GLuint VAO_bbox_, VAO_bbox_faces_, vertex_buffer_bbox_, element_buffer_bbox_, element_buffer_bbox_faces_;
        glm::mat4 bbox_transform_;

        void GenerateBoundingBox(std::vector<game_engine::graphics::Vertex_t>& vertices);
    };



    class OpenGLTriangle : public OpenGLObject {
        friend class OpenGLRenderer;
    public:
        OpenGLTriangle();

        int Init(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    };



    class OpenGLSkyboxCube {
        friend class OpenGLRenderer;
    public:
        OpenGLSkyboxCube();

        int Init();

        void Render();

    private:
        bool is_inited_ = false;

        size_t total_indices_;
        GLuint VAO_, vertex_buffer_, element_buffer_;
    };

}
}
}

#endif
