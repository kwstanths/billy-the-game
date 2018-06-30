#include "Renderer.hpp"

namespace gl = game_engine::graphics::opengl;

namespace game_engine {
namespace graphics {

    Renderer::Renderer() {
        renderer_ = new gl::OpenGLRenderer();
        is_inited_ = false;
    }

    int Renderer::Init(gl::OpenGLContext * context) {

        renderer_->Init(context);

        is_inited_ = true;
        return 0;
    }

    bool Renderer::IsInited() {
        return is_inited_;
    }

    void Renderer::SetView(opengl::OpenGLCamera * camera) {
        renderer_->SetView(camera);
    }

    void Renderer::StartFrame() {
        /* TODO Pre frame drawing initialization */
    }

    int Renderer::Draw(GraphicsObject * rendering_object) {
        renderer_->Draw(rendering_object->object_, 
            rendering_object->diffuse_texture_, 
            rendering_object->specular_texture_, 
            rendering_object->model_, 
            rendering_object->object_material_);
        return 0;
    }

    int Renderer::Draw(opengl::OpenGLObject * object, opengl::OpenGLTexture * texture, glm::mat4 model) {
        renderer_->Draw(object, texture, model);
        return 0;
    }

    int Renderer::AddLight(glm::vec3 position, graphics::LightProperties_t light_properties, Attenuation_t attenuation) {
        return renderer_->SetLight(position,
            light_properties.ambient_, light_properties.diffuse_, light_properties.specular_,
            attenuation.constant_, attenuation.linear_, attenuation.quadratic_);
    }

    int Renderer::AddDirectionalLight(glm::vec3 direction, LightProperties_t light_properties) {
        return renderer_->SetDirectionalLight(direction, light_properties.ambient_, light_properties.diffuse_, light_properties.specular_);
    }

    int Renderer::AddSpotLight(glm::vec3 position, glm::vec3 direction, float angle, LightProperties_t light_properties, Attenuation_t attenuation) {
        return renderer_->SetSpotLight(position, direction, GetRadians(angle),
            light_properties.ambient_, light_properties.diffuse_, light_properties.specular_,
            attenuation.constant_, attenuation.linear_, attenuation.quadratic_);
    }

    int Renderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
        return renderer_->Draw2DText(text, x, y, scale, color);
    }

}
}