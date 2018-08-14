#include "Mesh.hpp"

#include "AssetManager.hpp"

namespace gl = game_engine::graphics::opengl;

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

            /* Try to find if it was previously allocated */
            AssetManager & asset_manager = AssetManager::GetInstance();
            std::string texture_path = textures_[i].path_;
            gl::OpenGLTexture * previously_allocated_texture = asset_manager.FindTexture(texture_path);
            if (!previously_allocated_texture) {
                previously_allocated_texture = new gl::OpenGLTexture();
                previously_allocated_texture->Init(textures_[i].path_, textures_[i].type_);
                asset_manager.InsertTexture(texture_path, previously_allocated_texture);
            }

            opengl_textures_.push_back(previously_allocated_texture);
        }

        is_inited_ = true;
        return 0;
    }

    int Mesh::Destroy() {

        if (!is_inited_) return -1;

        opengl_object_.Destroy();
        for (size_t i = 0; i < textures_.size(); i++)
            opengl_textures_[i]->Destroy();
        opengl_textures_.clear();

        is_inited_ = false;
        return 0;
    }

    bool Mesh::IsInited() {
        return is_inited_;
    }


}
}