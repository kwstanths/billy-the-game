#ifndef __Mesh_hpp__
#define __Mesh_hpp__

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"

#include "Material.hpp"

namespace game_engine {
namespace graphics {

    class Mesh {
        friend class Renderer;
    public:
        Mesh();

        int Init(std::vector<Vertex_t> & vertices, 
            std::vector<unsigned int> & indices, 
            Material * material
        );

        int Destroy();

        bool IsInited();

        game_engine::Real_t GetBoundigBoxVolume();

        void SetMaterial(Material * material);

    private:
        bool is_inited_;

        std::vector<Vertex_t> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<Texture_t> textures_;
        opengl::OpenGLObject opengl_object_;
        
        Material * material_;
    };

}
}

#endif