#include "Mesh.hpp"

#include "AssetManager.hpp"
#include "game_engine/core/ErrorCodes.hpp"

namespace gl = game_engine::graphics::opengl;

namespace game_engine {
namespace graphics {

    Mesh::Mesh() {
        is_inited_ = false;
    }

    int Mesh::Init(std::vector<Vertex_t> & vertices, 
        std::vector<unsigned int> & indices
    ) {
    
        vertices_ = vertices;
        indices_ = indices;

        /* opengl object */
        opengl_object_.Init(vertices_, indices_);

        is_inited_ = true;
        return 0;
    }

    int Mesh::Destroy() {

        if (!is_inited_) return -1;

        opengl_object_.Destroy();

        is_inited_ = false;
        return 0;
    }

    bool Mesh::IsInited() {
        return is_inited_;
    }

    game_engine::Real_t Mesh::GetBoundigBoxVolume() {
        return opengl_object_.GetBoundingBoxVolume();
    }


}
}