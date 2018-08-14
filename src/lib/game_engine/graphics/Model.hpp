#ifndef __Model_hpp__
#define __Model_hpp__

#include "assimp/scene.h"

#include "Mesh.hpp"

namespace game_engine {
namespace graphics {

    class Model {
        friend class Renderer;
    public:
        Model();

        int Init(std::string model_file_path);

        int Destroy();

        bool IsInited();

    private:
        bool is_inited_;

        std::vector<Mesh *> meshes_;
        std::string directory_;

        int LoadModel(std::string file_path);

        int ProcessNode(aiNode *node, const aiScene *scene);

        Mesh * ProcessMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture_t> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, int texture_type);
    };

}
}

#endif