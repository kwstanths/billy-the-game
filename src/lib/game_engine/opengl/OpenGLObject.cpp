#include "OpenGLObject.hpp"

#include <vector>

#include "OpenGLMath.hpp"

#include "../ObjLoad.hpp"

#include "debug_tools/Console.hpp"

namespace game_engine{

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

        triangles_ = vertices.size();
        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &uv_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);       

        is_inited_ = true;
        return 0;
    }

    int OpenGLObject::Destroy() {
        if (!is_inited_) return -1;

        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteBuffers(1, &uv_buffer_);

        is_inited_ = false;
        return 0;
    }

    bool OpenGLObject::IsInited() {
        return is_inited_;
    }

    GLuint OpenGLObject::GetVertexBufferID()
    {
        return vertex_buffer_;
    }

    GLuint OpenGLObject::GetUVBufferID()
    {
        return uv_buffer_;
    }

    size_t OpenGLObject::GetNoFTriangles() {
        if (!is_inited_) return 0;
        
        return triangles_;
    }

}
