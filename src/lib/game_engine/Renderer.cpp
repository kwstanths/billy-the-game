#include "Renderer.hpp"


namespace game_engine {

    Renderer::Renderer() {
        renderer_ = new OpenGLRenderer();
        is_inited_ = false;
    }

    int Renderer::Init(OpenGLContext * context) {

        renderer_->Init(context);

        is_inited_ = true;
        return 0;
    }

    bool Renderer::IsInited() {
        return is_inited_;
    }

    int Renderer::Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model) {
        renderer_->Draw(object, texture, model);
        return 0;
    }

    int Renderer::AddLight(glm::vec3 position, glm::vec3 color) {
        return renderer_->SetLight(position, color);
    }

    int Renderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
        return renderer_->Draw2DText(text, x, y, scale, color);
    }


}