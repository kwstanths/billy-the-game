#ifndef __Mesh_hpp__
#define __Mesh_hpp__

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"

#include "GraphicsTypes.hpp"

namespace game_engine {
namespace graphics {

    class Mesh {
        friend class Renderer;
    public:
        Mesh();

        int Init(std::vector<Vertex_t> & vertices, 
            std::vector<unsigned int> & indices, 
            std::vector<Texture_t> & textures,
            Material_t & mat
        );

        int Destroy();

        bool IsInited();

    private:
        bool is_inited_;

        std::vector<Vertex_t> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<Texture_t> textures_;
        Material_t mat_;

        opengl::OpenGLObject opengl_object_;
        std::vector<opengl::OpenGLTexture *> opengl_textures_;

    };

}
}

#endif