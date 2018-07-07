#include "GraphicsObject.hpp"
#include "Renderer.hpp"

#include"game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace game_engine {
namespace graphics {

    GraphicsObject::GraphicsObject() {

        translation_matrix_ = GetTranslateMatrix(0, 0, 0);

        is_inited_ = false;
    }

    int GraphicsObject::Init(float x, float y, float z, opengl::OpenGLObject * object, opengl::OpenGLTexture * diffuse_texture, opengl::OpenGLTexture * specular_texture) {

        if (object == nullptr) return Error::ERROR_OBJECT_NOT_INIT;
        if (diffuse_texture == nullptr) return Error::ERROR_TEXTURE_NOT_INIT;
        if (!object->IsInited()) return Error::ERROR_OBJECT_NOT_INIT;
        if (!diffuse_texture->IsInited()) return Error::ERROR_TEXTURE_NOT_INIT;

        translation_matrix_ = GetTranslateMatrix(x, y, z);
        scale_matrix_ = GetScaleMatrix(1.0f, 1.0f, 1.0f);

        object_ = object;
        diffuse_texture_ = diffuse_texture;
        specular_texture_ = specular_texture;

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

        model_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
        renderer->Draw(this);
    }

    void GraphicsObject::SetPosition(float x, float y, float z) {
        /* Set the translation matrix */
        translation_matrix_ = GetTranslateMatrix(x, y, z);
    }

    void GraphicsObject::Scale(float scale_x, float scale_y, float scale_z) {
        /* Set the scaling matrix */
        scale_matrix_ = GetScaleMatrix(scale_x, scale_y, scale_z);
    }

    void GraphicsObject::Rotate(float angle, size_t axis) {
        glm::vec3 rotation_axis;

        if (axis == 0) rotation_matrix_ = GetRotateMatrix(angle, 1, 0, 0);
        else if (axis == 1) rotation_matrix_ = GetRotateMatrix(angle, 0, 1, 0);
        else if (axis == 2) rotation_matrix_ = GetRotateMatrix(angle, 0, 0, 1);

    }

    void GraphicsObject::SetMaterial(Material_t mtl) {
        object_material_ = mtl;
    }

}
}