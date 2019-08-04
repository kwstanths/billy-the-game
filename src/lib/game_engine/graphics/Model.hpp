#ifndef __Model_hpp__
#define __Model_hpp__

#include "Mesh.hpp"

namespace game_engine {
namespace graphics {

    class Model {
        friend class Renderer;
    public:
        Model();

        int Init(std::string model_file_path);

        int Init(std::vector<Mesh *> meshes);

        int Destroy();

        bool IsInited();

        size_t GetNumberOfMeshes();

    private:
        bool is_inited_;

        std::vector<Mesh *> meshes_;
    };

}
}

#endif