#ifndef __Model_hpp__
#define __Model_hpp__

#include "Mesh.hpp"
#include "Material.hpp"

namespace game_engine {
namespace graphics {

    class Model {
        friend class Renderer;
    public:
        Model();

        int Init(std::string model_file_path);

        int Init(std::vector<Mesh *> meshes, std::vector<Material *> materials_);

        int Destroy();

        bool IsInited();

        size_t GetNumberOfMeshes();

    private:
        bool is_inited_;

        std::vector<Mesh *> meshes_;
        std::vector<Material *> materials_;
    };

}
}

#endif