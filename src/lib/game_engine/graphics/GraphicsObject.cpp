#include "GraphicsObject.hpp"
#include "Renderer.hpp"

#include "game_engine/ErrorCodes.hpp"
#include "game_engine/math/Matrices.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "AssetManager.hpp"

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

        is_inited_ = true;
        return 0;
    }

    int GraphicsObject::Destroy() {

        is_inited_ = false;
        return 0;
    }

    bool GraphicsObject::IsInited() {
        return is_inited_;
    }

    void GraphicsObject::Draw(Renderer * renderer) {

        SetModelMatrix();
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

    void GraphicsObject::SetRotation(Real_t angle, glm::vec3 axis) {
        rotation_matrix_ = math::GetRotateMatrix(angle, axis.x, axis.y, axis.z);
    }

    void GraphicsObject::Rotate(Real_t angle, glm::vec3 axis) {
        rotation_matrix_ *= math::GetRotateMatrix(angle, axis.x, axis.y, axis.z);
    }

    void GraphicsObject::SetModelMatrix() {
        model_matrix_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    }


}
}