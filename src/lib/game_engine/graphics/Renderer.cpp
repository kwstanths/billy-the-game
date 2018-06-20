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

    int Renderer::Draw(gl::OpenGLObject * object, gl::OpenGLTexture * texture, glm::mat4 model) {
        renderer_->Draw(object, texture, model);
        return 0;
    }

    int Renderer::AddLight(glm::vec3 position, graphics::LightProperties_t light_properties) {
        return renderer_->SetLight(position, light_properties.ambient_, light_properties.diffuse_, light_properties.specular_);
    }

    int Renderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
        return renderer_->Draw2DText(text, x, y, scale, color);
    }

}
}