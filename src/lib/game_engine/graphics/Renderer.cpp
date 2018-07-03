#include "Renderer.hpp"

#include "game_engine/ErrorCodes.hpp"


namespace gl = game_engine::graphics::opengl;

namespace game_engine {
namespace graphics {

    Renderer::Renderer() {
        context_ = new gl::OpenGLContext();
        renderer_ = new gl::OpenGLRenderer();
        is_inited_ = false;
    }

    int Renderer::Init(gl::OpenGLContextConfig_t config) {

        int ret = context_->Init(config);
        if (ret != 0) return ret;

        renderer_->Init(context_);

        is_inited_ = true;
        return 0;
    }

    int Renderer::Destroy() {
        /* TODO */
        return -1;
    }

    bool Renderer::IsInited() {
        return is_inited_;
    }

    void Renderer::SetView() {
        renderer_->SetView(camera_);
    }

    void Renderer::StartFrame() {
        context_->ClearColor();
    }

    void Renderer::EndFrame() {
        context_->SwapBuffers();

    }

    void Renderer::SetWindowSize(size_t width, size_t height) {

        context_->SetWindowSize(width, height);
    }

    KeyControls_t Renderer::GetControlInput() {
        return context_->GetControlsInput();
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

    int Renderer::AddSpotLight(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, 
        LightProperties_t light_properties, Attenuation_t attenuation) 
    {
        return renderer_->SetSpotLight(position, direction, GetRadians(inner_angle), GetRadians(outer_angle),
            light_properties.ambient_, light_properties.diffuse_, light_properties.specular_,
            attenuation.constant_, attenuation.linear_, attenuation.quadratic_);
    }

    int Renderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
        return renderer_->Draw2DText(text, x, y, scale, color);
    }

    int Renderer::SetCamera(gl::OpenGLCamera * camera) {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        camera_ = camera;
        camera->Init(context_);
    }

}
}