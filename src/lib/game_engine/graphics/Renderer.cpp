#include "Renderer.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/ErrorCodes.hpp"


namespace math = game_engine::math;
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

        point_lights_to_draw_.Init(GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS);
        objects_to_draw_.Init(GAME_ENGINE_RENDERER_MAX_OBJECTS);
        text_to_draw_.Init(512);

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
        
        point_lights_to_draw_.Clear();
        objects_to_draw_.Clear();
    }

    void Renderer::EndFrame() {

        FlushDrawCalls();

        context_->SwapBuffers();

    }

    void Renderer::SetWindowSize(size_t width, size_t height) {

        context_->SetWindowSize(width, height);
    }

    KeyControls_t Renderer::GetControlInput() {
        return context_->GetControlsInput();
    }

    int Renderer::Draw(GraphicsObject * rendering_object) {
        if (objects_to_draw_.IsFull()) {
            dt::Console(dt::WARNING, "Renderer::Draw(): Maximum number of objects reached");
            return -1;
        }

        objects_to_draw_.Push(rendering_object);
        return 0;
    }

    int Renderer::DrawSimple(GraphicsObject * rendering_object) {

        rendering_object->SetModelMatrix();

        std::vector<Mesh *> & meshes = rendering_object->model_->meshes_;

        for (size_t i = 0; i < meshes.size(); i++) {
            Mesh * mesh = meshes[i];
            gl::OpenGLTexture * texture = mesh->opengl_textures_.at(0);
            renderer_->Draw(&(mesh->opengl_object_), texture, rendering_object->model_matrix_);
        }
        return 0;
    }

    int Renderer::DrawLineXY(math::Point2D start, math::Point2D stop, float z_height, float size, glm::vec3 color) {
        return renderer_->DrawLineXY(start.x_, start.y_, stop.x_, stop.y_, z_height, size, color);
    }

    int Renderer::DrawRectangleXY(math::Rectangle2D rect, float z_height, float size, glm::vec3 color) {

        if (!renderer_->IsInited()) return -1;

        renderer_->DrawLineXY(rect.A_.x_, rect.A_.y_, rect.B_.x_, rect.B_.y_, z_height, size, color);
        renderer_->DrawLineXY(rect.B_.x_, rect.B_.y_, rect.C_.x_, rect.C_.y_, z_height, size, color);
        renderer_->DrawLineXY(rect.C_.x_, rect.C_.y_, rect.D_.x_, rect.D_.y_, z_height, size, color);
        renderer_->DrawLineXY(rect.D_.x_, rect.D_.y_, rect.A_.x_, rect.A_.y_, z_height, size, color);

        return 0;
    }

    int Renderer::AddDirectionalLight(glm::vec3 direction, LightProperties_t light_properties) {
        return renderer_->SetDirectionalLight(direction, light_properties.ambient_, light_properties.diffuse_, light_properties.specular_);
    }

    int Renderer::AddSpotLight(glm::vec3 position, glm::vec3 direction, Real_t inner_angle, Real_t outer_angle, 
        LightProperties_t light_properties, Attenuation_t attenuation) 
    {
        return renderer_->SetSpotLight(position, direction, math::GetRadians(inner_angle), math::GetRadians(outer_angle),
            light_properties.ambient_, light_properties.diffuse_, light_properties.specular_,
            attenuation.constant_, attenuation.linear_, attenuation.quadratic_);
    }

    int Renderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {

        if (text_to_draw_.Items() >= text_to_draw_.Size()) {
            dt::Console(dt::WARNING, "Renderer::Draw2DText(): Maximum number reached");
            return -1;
        }

        text_to_draw_.Push({ text, x, y, scale, color });
    }

    int Renderer::SetCamera(gl::OpenGLCamera * camera) {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        camera_ = camera;
        camera->Init(context_);

        return 0;
    }

    int Renderer::AddPointLight(PointLight_t * light) {
        if (point_lights_to_draw_.Items() >= GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS) {
            dt::Console(dt::WARNING, "Renderer::AddPointLight(): Maximum number of lights reached");
            return -1;
        }

        point_lights_to_draw_.Push(light);
        return 0;
    }

    void Renderer::FlushDrawCalls() {

        size_t number_of_point_lights_= point_lights_to_draw_.Items();
        renderer_->SetPointLightsNumber(number_of_point_lights_);
        for (size_t i = 0; i < number_of_point_lights_; i++) {
            std::string index = std::to_string(i);

            PointLight_t * light;
            point_lights_to_draw_.Get(light);
            renderer_->SetPointLight(index,
                light->position_,
                light->properties_.ambient_,
                light->properties_.diffuse_,
                light->properties_.specular_,
                light->attenutation_.constant_,
                light->attenutation_.linear_,
                light->attenutation_.quadratic_
            );
        }

        size_t number_of_objects_to_draw_ = objects_to_draw_.Items();
        for (size_t i = 0; i < number_of_objects_to_draw_; i++) {
            GraphicsObject * rendering_object;
            objects_to_draw_.Get(rendering_object);
            
            rendering_object->SetModelMatrix();

            std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
            for (size_t i = 0; i < meshes.size(); i++) {
                Mesh * mesh = meshes[i];
                renderer_->Draw(mesh->opengl_object_, mesh->opengl_textures_, rendering_object->model_matrix_, mesh->mat_);
            }
        }

        size_t number_of_text_to_draw = text_to_draw_.Items();
        for (size_t i = 0; i < number_of_text_to_draw; i++) {
            TEXT_DRAW_t temp;
            text_to_draw_.Get(temp);
        
            renderer_->Draw2DText(temp.text_, temp.x, temp.y, temp.scale, temp.color);
        }

    }

}
}