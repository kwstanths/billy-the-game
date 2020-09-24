#include "GraphicsObject.hpp"
#include "Renderer.hpp"

#include "game_engine/core/ErrorCodes.hpp"
#include "game_engine/core/FileSystem.hpp"
#include "game_engine/math/Matrices.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "AssetManager.hpp"
#include "AssimpHelp.hpp"

namespace math = game_engine::math;

namespace game_engine {
namespace graphics {

    GraphicsObject::GraphicsObject() {

        translation_matrix_ = math::GetTranslateMatrix(0, 0, 0);

        is_inited_ = false;
    }

    int GraphicsObject::Init(Real_t x, Real_t y, Real_t z, std::string model_file) {

        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
        scale_matrix_ = math::GetScaleMatrix(1.0f, 1.0f, 1.0f);

        /* If model already loaded, then use the same */
        AssetManager& asset_manager = AssetManager::GetInstance();
        std::string assets_dir = FileSystem::GetInstance().GetDirectoryAssets();
        model_ = asset_manager.FindModel(assets_dir + model_file);
        if (model_ == nullptr) {
            /* Else load it, and insert it in the asset manager */
            model_ = new Model();
            model_->Init(model_file);
            asset_manager.InsertModel(assets_dir + model_file, model_);
        }

        model_materials_ = std::vector<Material *>(model_->GetNumberOfMeshes(), nullptr);

        is_inited_ = true;
        return 0;
    }

    int GraphicsObject::InitObjectAtlas(std::string file_name) {

        std::vector<AssimpData_t> model_data;
        int ret = ProcessObjectAtlas(file_name, model_data);

        std::string directory = FileSystem::GetInstance().GetDirectoryAssets();
        std::string file_path = directory + file_name.substr(0, file_name.find_last_of("."));

        AssetManager& asset_manager = AssetManager::GetInstance();
        for (size_t i = 0; i < model_data.size(); i++) {
            Model * new_model = new Model();
            new_model->Init({ model_data[i].mesh_ }, { model_data[i].material_ });
            asset_manager.InsertModel(file_path + "_" + std::to_string(i) + ".obj", new_model);
        }

        return 0;
    }

    int GraphicsObject::Destroy() {

        /* DONT Destroy or delete model_, It stil might be used by other objects */

        is_inited_ = false;
        return 0;
    }

    bool GraphicsObject::IsInited() {
        return is_inited_;
    }

    void GraphicsObject::Draw(Renderer * renderer) {

        renderer->Draw(this);
    }

    void GraphicsObject::SetPosition(Real_t x, Real_t y, Real_t z) {
        /* Set the translation matrix */
        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
    }

    void GraphicsObject::Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z) {
        /* Set the scaling matrix */
        scale_matrix_ = math::GetScaleMatrix(scale_x, scale_y, scale_z);
    }

    void GraphicsObject::SetRotation(glm::mat4 rotation) {
        rotation_matrix_ = rotation;
    }

    glm::mat4 GraphicsObject::GetRotation()
    {
        return rotation_matrix_;
    }

    void GraphicsObject::Rotate(Real_t angle, glm::vec3 axis) {
        rotation_matrix_ *= math::GetRotateMatrix(angle, axis.x, axis.y, axis.z);
    }

    void GraphicsObject::SetMaterial(Material * material, int mesh_index)
    {
        if (mesh_index == -1) {
            for (size_t i = 0; i < model_materials_.size(); i++) {
                model_materials_[i] = material;
            }
            return;
        } else if (mesh_index >= model_materials_.size()) {
            return;
        }

        model_materials_[mesh_index] = material;
    }

    void GraphicsObject::SetModelMatrix() {
        model_matrix_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    }


}
}