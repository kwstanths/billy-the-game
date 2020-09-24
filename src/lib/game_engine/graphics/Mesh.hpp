#ifndef __Mesh_hpp__
#define __Mesh_hpp__

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/opengl/OpenGLQuery.hpp"

namespace game_engine {
namespace graphics {

    class Mesh {
        friend class Renderer;
    public:
        Mesh();

        int Init(std::vector<Vertex_t> & vertices, 
            std::vector<unsigned int> & indices
        );

        int Destroy();

        bool IsInited();

        game_engine::Real_t GetBoundigBoxVolume();

    private:
        bool is_inited_;

        std::vector<Vertex_t> vertices_;
        std::vector<unsigned int> indices_;
        opengl::OpenGLObject opengl_object_;
    };

}
}

#endif