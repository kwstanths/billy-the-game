#include "Mesh.hpp"


namespace game_engine {
namespace graphics {

    Mesh::Mesh() {
        is_inited_ = false;
    }

    int Mesh::Init(std::vector<Vertex_t> & vertices, 
        std::vector<unsigned int> & indices, 
        std::vector<Texture_t> & textures,
        Material_t & mat
    ) {
    
        vertices_ = vertices;
        indices_ = indices;
        textures_ = textures;
        mat_ = mat;
        
        /* opengl object */
        opengl_object_.Init(vertices_, indices_);
        /* opengl textures */
        for (size_t i = 0; i < textures_.size(); i++) {
            opengl_textures_.push_back(new opengl::OpenGLTexture());
            opengl_textures_[i]->Init(textures_[i].path_, textures_[i].type_);
        }

        is_inited_ = true;
        return 0;
    }

    int Mesh::Destroy() {

        opengl_object_.Destroy();
        for (size_t i = 0; i < textures_.size(); i++)
            opengl_textures_[i]->Destroy();
        textures_.clear();

        is_inited_ = false;
        return 0;
    }

    bool Mesh::IsInited() {
        return is_inited_;
    }


}
}