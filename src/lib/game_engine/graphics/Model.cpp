#include "Model.hpp"

#include <algorithm>

#include "AssetManager.hpp"
#include "AssimpHelp.hpp"

namespace game_engine {
namespace graphics {

    Model::Model() {
        is_inited_ = false;
    }

    int Model::Init(std::string model_file_path) {
        
        std::vector<AssimpData_t> model_data;

        int ret = LoadModel(model_file_path, model_data);
        if (ret) return ret;

        /* Keep the meshes, return the materials */
        for (size_t i = 0; i < model_data.size(); i++) {
            meshes_.push_back(model_data[i].mesh_);
            materials_.push_back(model_data[i].material_);
        }

        is_inited_ = true;
        
        return 0;
    }

    int Model::Init(std::vector<Mesh*> meshes, std::vector<Material *> materials) {
        meshes_ = meshes;
        materials_ = materials;

        is_inited_ = true;
        return 0;
    }

    int Model::Destroy() {
        
        if (!is_inited_) return -1;

        for (size_t i = 0; i < meshes_.size(); i++) {
            Mesh * mesh = meshes_[i];
            mesh->Destroy();
            delete mesh;
        }

        is_inited_ = false;

        return 0;
    }

    bool Model::IsInited() {
        return is_inited_;
    }

    size_t Model::GetNumberOfMeshes() {
        return meshes_.size();
    }

}
}