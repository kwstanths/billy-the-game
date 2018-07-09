#ifndef __Mesh_hpp__
#define __Mesh_hpp__

#include <glm/glm.hpp>

namespace game_engine {
namespace graphics {

    /**
        A struct to hold vertex data
    */
    struct Vertex_t {
        glm::vec3 position_;
        glm::vec3 normal_;
        glm::vec2 uv_;
    };


    class Mesh {
    public:
        Mesh();

        int Init();

        int Destroy();

        bool IsInited();

    private:
        bool is_inited_;

    };

}
}

#endif