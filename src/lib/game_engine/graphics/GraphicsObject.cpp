#include "GraphicsObject.hpp"
#include "Renderer.hpp"

#include "game_engine/ErrorCodes.hpp"
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

    int GraphicsObject::Init(Real_t x, Real_t y, Real_t z, std::string model_file_path) {

        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
        scale_matrix_ = math::GetScaleMatrix(1.0f, 1.0f, 1.0f);

        /* If model already loaded, then use the same */
        AssetManager& asset_manager = AssetManager::GetInstance();
        model_ = asset_manager.FindModel(model_file_path);
        if (model_ == nullptr) {
            /* Else load it, and insert it in the asset manager */
            model_ = new Model();
            model_->Init(model_file_path);
            asset_manager.InsertModel(model_file_path, model_);
        }

        mesh_queries_ = std::vector<opengl::OpenGLQuery>(model_->GetNumberOfMeshes());
        for (size_t i = 0; i < model_->GetNumberOfMeshes(); i++)
            mesh_queries_[i].Init();

        is_inited_ = true;
        return 0;
    }

    int GraphicsObject::InitObjectAtlas(std::string file_name) {

        AssetManager& asset_manager = AssetManager::GetInstance();
        std::string file_path = file_name.substr(0, file_name.find_last_of("."));

        std::vector<Mesh *> meshes;
        int ret = ProcessTextureAtlas(file_name, meshes);

        for (size_t i = 0; i < meshes.size(); i++) {
            Model * new_model = new Model();
            new_model->Init({ meshes[i] });
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

    void GraphicsObject::SetModelMatrix() {
        model_matrix_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    }


}
}