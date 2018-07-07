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

    int OpenGLObject::Init(std::string object_path) {
        if (is_inited_) return -1;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        int ret = LoadObj(object_path.c_str(), vertices, uvs, normals);
        if (ret != 0) return ret;

        std::vector<unsigned short> indices;
        std::vector<glm::vec3> indexed_vertices;
        std::vector<glm::vec2> indexed_uvs;
        std::vector<glm::vec3> indexed_normals;
        IndexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uv_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &normal_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_);
        glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &element_buffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

        total_indices_ = indices.size();

        is_inited_ = true;
        return 0;
    }

    int OpenGLObject::Destroy() {
        if (!is_inited_) return -1;

        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &uv_buffer_);
        glDeleteBuffers(1, &normal_buffer_);
        glDeleteBuffers(1, &element_buffer_);

        is_inited_ = false;
        return 0;
    }

    bool OpenGLObject::IsInited() {
        return is_inited_;
    }

    GLuint OpenGLObject::GetVertexBufferID() {
        return vertex_buffer_;
    }

    GLuint OpenGLObject::GetUVBufferID() {
        return uv_buffer_;
    }

    GLuint OpenGLObject::GetNormalBufferID() {
        return normal_buffer_;
    }

    GLuint OpenGLObject::GetElementBufferID() {
        return element_buffer_;
    }

    size_t OpenGLObject::GetNoFElements() {
        if (!is_inited_) return 0;
        
        return total_indices_;
    }

    void OpenGLObject::IndexVBO(std::vector<glm::vec3>& in_vertices, 
        std::vector<glm::vec2>& in_uvs, 
        std::vector<glm::vec3>& in_normals, 
        std::vector<unsigned short>& out_indices, 
        std::vector<glm::vec3>& out_vertices, 
        std::vector<glm::vec2>& out_uvs, 
        std::vector<glm::vec3>& out_normals) 
    {
        std::map<PackedVertex, unsigned short> VertexToOutIndex;

        /* For each vertex */
        for (unsigned int i = 0; i<in_vertices.size(); i++) {

            PackedVertex packed_vertex = { in_vertices[i], in_uvs[i], in_normals[i] };

            /* Search if we already indexed one */
            bool found;
            unsigned short index;
            std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed_vertex);
            if (it == VertexToOutIndex.end()) {
                found = false;
            } else {
                index = it->second;
                found = true;
            }


            if (found) {
                /* If a similar vertex was already found just return its index */
                out_indices.push_back(index);
            }
            else {
                /* If not, push into the indexed structures */
                out_vertices.push_back(in_vertices[i]);
                out_uvs.push_back(in_uvs[i]);
                out_normals.push_back(in_normals[i]);
                unsigned short newindex = (unsigned short)out_vertices.size() - 1;
                out_indices.push_back(newindex);
                VertexToOutIndex[packed_vertex] = newindex;
            }
        }
    }

    
}
}
}
