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

        /* Create an opengl renderer */
        {
            int ret = context_->Init(config);
            if (ret != 0) return ret;

            renderer_->Init(context_);
        }

        point_lights_to_draw_ = std::vector<PointLight_t>(NR_POINT_LIGHTS);

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

        number_of_point_lights_ = 0;
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
        for (size_t i = 0; i < rendering_object->meshes_.size(); i++) {
            Mesh * mesh = rendering_object->meshes_[i];
            renderer_->Draw(mesh->opengl_object_, mesh->opengl_textures_, rendering_object->model_, mesh->mat_);
        }
        return 0;
    }

    int Renderer::DrawSimple(GraphicsObject * rendering_object) {
        for (size_t i = 0; i < rendering_object->meshes_.size(); i++) {
            Mesh * mesh = rendering_object->meshes_[i];
            gl::OpenGLTexture * texture = mesh->opengl_textures_.at(0);
            renderer_->Draw(&(mesh->opengl_object_), texture, rendering_object->model_);
        }
        return 0;
    }

    int Renderer::AddPointLight(glm::vec3 position, graphics::LightProperties_t light_properties, Attenuation_t attenuation) {
        if (number_of_point_lights_ >= NR_POINT_LIGHTS) {
            dt::Console(dt::WARNING, "Renderer::AddPointLight(): Maximum number of lights reached");
            return -1;
        }
        point_lights_to_draw_.at(number_of_point_lights_++) = PointLight_t(position, light_properties, attenuation);
        return 0;
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

        return 0;
    }

    void Renderer::FlushDrawsCalls() {
        renderer_->SetPointLightsNumber(number_of_point_lights_);

        for (size_t i = 0; i < number_of_point_lights_; i++) {
            std::string index = std::to_string(i);
            renderer_->SetPointLight(index,
                point_lights_to_draw_[i].position_,
                point_lights_to_draw_[i].properties_.ambient_,
                point_lights_to_draw_[i].properties_.diffuse_,
                point_lights_to_draw_[i].properties_.specular_,
                point_lights_to_draw_[i].attenutation_.constant_,
                point_lights_to_draw_[i].attenutation_.linear_,
                point_lights_to_draw_[i].attenutation_.quadratic_
            );
        }

    }

}
}