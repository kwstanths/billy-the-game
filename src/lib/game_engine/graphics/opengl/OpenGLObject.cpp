#include "OpenGLObject.hpp"

#include "game_engine/math/Matrices.hpp"
#include "game_engine/FileIO.hpp"

#include "debug_tools/Console.hpp"

#include <map>

namespace game_engine { namespace graphics { namespace opengl {

    OpenGLObject::OpenGLObject() {
        is_inited_ = false;
    }
    
    int OpenGLObject::Init(std::vector<Vertex_t> & vertices, std::vector<unsigned int> & indices, bool generate_bbox_info) {
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
    
        /* Generate geometry for the bounding box */
        if (generate_bbox_info) GenerateBoundingBox(vertices);
    
        is_inited_ = true;
        return 0;
    }

    void OpenGLObject::SetVertices(std::vector<game_engine::graphics::Vertex_t>& vertices) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex_t), &vertices[0], GL_STATIC_DRAW);
    }
    
    void OpenGLObject::SetupAttributes(OpenGLShaderVerticesColor * shader) {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
    }
    
    void OpenGLObject::SetupAttributes(OpenGLShaderGBuffer * shader) {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
        /* Attribute number 1 is the object's uv coordinates */
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_uv), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, uv_));
        /* Attribute number 2 is the object's normals */
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_normal), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, normal_));
    }

    void OpenGLObject::SetupAttributes(OpenGLShaderShadowMap * shader) {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
    }

    void OpenGLObject::SetupAttributes(OpenGLShaderDrawNormals * shader)
    {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->attr_vertex_position_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
        /* Attribute number 2 is the object's normals */
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(shader->attr_vertex_normal_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, normal_));
    }

    void OpenGLObject::SetupAttributes(OpenGLShaderDisplacement * shader)
    {
        /* Set shader layout attributes */
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)0);
        /* Attribute number 1 is the object's uv coordinates */
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_uv), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, uv_));
        /* Attribute number 2 is the object's normals */
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(shader->GetAttributeLocation(shader_vertex_normal), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (void*)offsetof(Vertex_t, normal_));
    }
    
    void OpenGLObject::Render() {
        /* Draw with index buffer */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_);
        glDrawElements(GL_TRIANGLES, total_indices_, GL_UNSIGNED_INT, (void*)0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void OpenGLObject::RenderBoundingBoxLines() {
        /* Draw bounding box lines */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_bbox_);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void OpenGLObject::RenderBoundingBoxFaces() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_bbox_faces_);
        glDrawElements(GL_TRIANGLES, total_indices_bbox_faces_, GL_UNSIGNED_SHORT, (void*)0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    int OpenGLObject::Destroy() {
        if (!is_inited_) return -1;
    
        /* Delete buffers for the object */
        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &element_buffer_);
        glDeleteVertexArrays(1, &VAO_);
    
        /* Delete buffers for the bounding box */
        glDeleteBuffers(1, &vertex_buffer_bbox_);
        glDeleteBuffers(1, &element_buffer_bbox_);
        glDeleteBuffers(1, &element_buffer_bbox_faces_);
        glDeleteVertexArrays(1, &VAO_bbox_);
        glDeleteVertexArrays(1, &VAO_bbox_faces_);
    
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
    
    game_engine::Real_t OpenGLObject::GetBoundingBoxVolume() {
        return std::abs(max_x_ - min_x_) * std::abs(max_y_ - min_y_) * std::abs(max_z_ - min_z_);
    }
    
    void OpenGLObject::GenerateBoundingBox(std::vector<game_engine::graphics::Vertex_t>& vertices) {
    
        /* VAO_bbox_ holds the VAO for a bounding box without faces */
        glGenVertexArrays(1, &VAO_bbox_);
        glBindVertexArray(VAO_bbox_);
    
        /* A simple cube 1x1x1, centered on origin */
        GLfloat cube_vertices[] = {
          -0.5, -0.5, -0.5, /* 0 */
           0.5, -0.5, -0.5, /* 1 */
           0.5,  0.5, -0.5, /* 2 */
          -0.5,  0.5, -0.5, /* 3 */
          -0.5, -0.5,  0.5, /* 4 */
           0.5, -0.5,  0.5, /* 5 */
           0.5,  0.5,  0.5, /* 6 */
          -0.5,  0.5,  0.5, /* 7 */
        };
        glGenBuffers(1, &vertex_buffer_bbox_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_bbox_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        GLushort cube_elements[] = {
          0, 1, 2, 3,
          4, 5, 6, 7,
          0, 4, 1, 5, 
          2, 6, 3, 7
        };
        glGenBuffers(1, &element_buffer_bbox_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_bbox_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
        /* VAO_bbox_faces_ holds the VAO for a bounding box with faces */
        glGenVertexArrays(1, &VAO_bbox_faces_);
        glBindVertexArray(VAO_bbox_faces_);
    
        GLushort cube_elements_faces[] = {
            // front
            1, 2, 6,
            6, 5, 1,
            // right
            2, 3, 7,
            7, 6, 2,
            // back
            7, 3, 0,
            0, 4, 7,
            // left
            0, 5, 4,
            0, 1, 5,
            // bottom
            1, 0, 3,
            1, 3, 2,
            // top
            4, 5, 6,
            4, 6, 7
        };
        total_indices_bbox_faces_ = 36;
        glGenBuffers(1, &element_buffer_bbox_faces_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_bbox_faces_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements_faces), cube_elements_faces, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
        /* Calculate bbox sizes */
        min_x_ = max_x_ = vertices[0].position_.x;
        min_y_ = max_y_ = vertices[0].position_.y;
        min_z_ = max_z_ = vertices[0].position_.z;
        for (size_t i = 0; i < vertices.size(); i++) {
            if (vertices[i].position_.x < min_x_) min_x_ = vertices[i].position_.x;
            if (vertices[i].position_.x > max_x_) max_x_ = vertices[i].position_.x;
            if (vertices[i].position_.y < min_y_) min_y_ = vertices[i].position_.y;
            if (vertices[i].position_.y > max_y_) max_y_ = vertices[i].position_.y;
            if (vertices[i].position_.z < min_z_) min_z_ = vertices[i].position_.z;
            if (vertices[i].position_.z > max_z_) max_z_ = vertices[i].position_.z;
        }
        glm::vec3 size = glm::vec3(max_x_ - min_x_, max_y_ - min_y_, max_z_ - min_z_);
        glm::vec3 center = glm::vec3((min_x_ + max_x_) / 2, (min_y_ + max_y_) / 2, (min_z_ + max_z_) / 2);
        bbox_transform_ = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);
    
    }

    OpenGLTriangle::OpenGLTriangle() {
        is_inited_ = false;
    }

    int OpenGLTriangle::Init(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {

        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

        /* TODO QUICK HACK!!!!, FIX ME!!!!, I DON'T BELONG HERE!!! */
        if (normal.y < 0) normal.y *= -1;

        std::vector<Vertex_t> vertices;
        vertices.push_back(Vertex_t(v1, normal, glm::vec2()));
        vertices.push_back(Vertex_t(v2, normal, glm::vec2()));
        vertices.push_back(Vertex_t(v3, normal, glm::vec2()));

        std::vector<unsigned int> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        int ret = OpenGLObject::Init(vertices, indices, true);

        return ret;
    }

}
}
}
