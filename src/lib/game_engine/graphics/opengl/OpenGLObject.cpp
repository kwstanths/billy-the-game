#include "OpenGLObject.hpp"

#include "game_engine/math/Matrices.hpp"
#include "game_engine/FileIO.hpp"

#include "debug_tools/Console.hpp"

#include <map>

namespace game_engine{
namespace graphics {
namespace opengl {

    OpenGLObject::OpenGLObject() {        
        is_inited_ = false;
    }

    int OpenGLObject::Init(std::vector<Vertex_t> & vertices, std::vector<unsigned int> & indices) {
        if (is_inited_) return -1;

        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);

        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex_t), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &element_buffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        total_indices_ = indices.size();

        is_inited_ = true;
        return 0;
    }

    void OpenGLObject::SetupAttributes(OpenGLShaderMain * shader) {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
        /* Attribute number 1 is the object's uv coordinates */
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_uv), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, uv_));
        /* Attribute number 2 is the object's normals */
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_normal), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, normal_));
    }

    void OpenGLObject::SetupAttributes(OpenGLShaderModelTexture * shader) {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
        /* Attribute number 1 is the object's uv coordinates */
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_uv), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, uv_));
        /* Attribute number 2 is the object's normals */
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_normal), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, normal_));
    }

    void OpenGLObject::SetupAttributes(OpenGLShaderVerticesColor * shader) {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_main_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
    }

    int OpenGLObject::Destroy() {
        if (!is_inited_) return -1;

        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &element_buffer_);
        glDeleteVertexArrays(1, &VAO_);

        is_inited_ = false;
        return 0;
    }

    bool OpenGLObject::IsInited() {
        return is_inited_;
    }

    GLuint OpenGLObject::GetVertexBufferID() {
        return vertex_buffer_;
    }

    GLuint OpenGLObject::GetElementBufferID() {
        return element_buffer_;
    }

    size_t OpenGLObject::GetNoFElements() {
        if (!is_inited_) return 0;
        
        return total_indices_;
    }

}
}
}
