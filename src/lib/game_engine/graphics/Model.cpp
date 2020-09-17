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
        
        int ret = LoadModel(model_file_path, meshes_);
        if (ret) return ret;

        /**
            Sort meshes with decreasing volume, this means that the render will see the meshes in this order
            it may help in occlusion queries, we render the big occluders first
        */
        std::sort(meshes_.begin(), meshes_.end(),
            [](Mesh * a, Mesh * b) -> bool {
            return a->GetBoundigBoxVolume() > b->GetBoundigBoxVolume();
        });

        is_inited_ = true;
        
        return 0;
    }

    int Model::Init(std::vector<Mesh*> meshes) {
        meshes_ = meshes;
        
        /**
            Sort meshes with decreasing volume, this means that the render will see the meshes in this order
            it may help in occlusion queries, we render the big occluders first
        */
        std::sort(meshes_.begin(), meshes_.end(),
            [](Mesh * a, Mesh * b) -> bool {
            return a->GetBoundigBoxVolume() > b->GetBoundigBoxVolume();
        });

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

    void Model::SetMaterial(Material * material, int mesh_index)
    {
        if (mesh_index == -1) {
            for (size_t i = 0; i < meshes_.size(); i++) {
                meshes_[i]->SetMaterial(material);
            }
        } else {
            if (!(mesh_index > meshes_.size())) {
                meshes_[mesh_index]->SetMaterial(material);
            }
        }
    }
    

}
}