#include "Renderer.hpp"

#include "opengl/OpenGLIncludes.hpp"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/ErrorCodes.hpp"
#include "game_engine/ConfigurationFile.hpp"

#include "game_engine/graphics/Frustum.hpp"

#include "glm/gtc/type_ptr.hpp"

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
        //objects_to_draw_.Init(GAME_ENGINE_RENDERER_MAX_OBJECTS);
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
        
        renderer_->g_buffer_->Bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer_->g_buffer_->UnBind();

        point_lights_to_draw_.Clear();
        objects_to_draw_.clear();
        text_to_draw_.Clear();
    }

    void Renderer::EndFrame() {
        /* Set camera parameters */
        Renderer::SetView();

        /* Flush drawing calls */
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
        //if (objects_to_draw_.IsFull()) {
        //    dt::Console(dt::WARNING, "Renderer::Draw(): Maximum number of objects reached");
        //    return -1;
        //}

        objects_to_draw_.push_back(rendering_object);
        return 0;
    }

    int Renderer::DrawSimple(GraphicsObject * rendering_object) {

        rendering_object->SetModelMatrix();

        std::vector<Mesh *> & meshes = rendering_object->model_->meshes_;

        for (size_t i = 0; i < meshes.size(); i++) {
            Mesh * mesh = meshes[i];
            gl::OpenGLTexture * texture = mesh->opengl_textures_.at(0);
            renderer_->Draw(mesh->opengl_object_, texture, rendering_object->model_matrix_);
        }
        return 0;
    }

    int Renderer::DrawLineXY(math::Point2D start, math::Point2D stop, float z_height, float size, glm::vec3 color) {

        renderer_->g_buffer_->Bind();
        renderer_->DrawLine(glm::vec3(start.x(), start.y(), z_height), glm::vec3(start.x(), start.y(), z_height));
        renderer_->g_buffer_->UnBind();

        return 0;
    }

    int Renderer::DrawRectangleXY(math::Rectangle2D rect, float z_height, float size, glm::vec3 color) {

        if (!renderer_->IsInited()) return -1;

        renderer_->g_buffer_->Bind();
        
        renderer_->DrawLine(glm::vec3(rect.A_.x(), rect.A_.y(), z_height), glm::vec3(rect.B_.x(), rect.B_.y(), z_height));
        renderer_->DrawLine(glm::vec3(rect.B_.x(), rect.B_.y(), z_height), glm::vec3(rect.C_.x(), rect.C_.y(), z_height));
        renderer_->DrawLine(glm::vec3(rect.C_.x(), rect.C_.y(), z_height), glm::vec3(rect.D_.x(), rect.D_.y(), z_height));
        renderer_->DrawLine(glm::vec3(rect.D_.x(), rect.D_.y(), z_height), glm::vec3(rect.A_.x(), rect.A_.y(), z_height));

        renderer_->g_buffer_->UnBind();

        return 0;
    }

    int Renderer::AddDirectionalLight(DirectionalLight * light) {
        return renderer_->SetDirectionalLight(light->direction_, light->ambient_, light->diffuse_, light->specular_);
    }

    int Renderer::AddSpotLight(SpotLight * light)  {
        return renderer_->SetSpotLight(light->position_, light->direction_, math::GetRadians(light->inner_angle_), math::GetRadians(light->outer_angle_),
            light->ambient_, light->diffuse_, light->specular_,
            light->attenuation_.constant_, light->attenuation_.linear_, light->attenuation_.quadratic_);
    }

    int Renderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
        if (text_to_draw_.Items() >= text_to_draw_.Size()) {
            dt::Console(dt::WARNING, "Renderer::Draw2DText(): Maximum number reached");
            return -1;
        }

        text_to_draw_.Push({ text, x, y, scale, color });
        return 0;
    }

    int Renderer::SetCamera(gl::OpenGLCamera * camera) {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        camera_ = camera;
        camera->Init(context_);

        return 0;
    }

    int Renderer::AddPointLight(PointLight * light) {
        if (point_lights_to_draw_.Items() >= GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS) {
            dt::Console(dt::WARNING, "Renderer::AddPointLight(): Maximum number of lights reached");
            return -1;
        }

        point_lights_to_draw_.Push(light);
        return 0;
    }

    void Renderer::FlushDrawCalls() {

        frr_render_mode = static_cast<RENDER_MODE>(ConfigurationFile::instance().GetRenderingMethod());
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Bind the gbuffer, and draw the geometry */
        renderer_->g_buffer_->Bind();


        switch (frr_render_mode)
        {
        case RENDER_MODE::REGULAR:
        {
            size_t number_of_objects_to_draw_ = objects_to_draw_.size();
            for (size_t i = 0; i < number_of_objects_to_draw_; i++) {
                GraphicsObject * rendering_object = objects_to_draw_.at(i);
                rendering_object->SetModelMatrix();
                std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
                for (size_t j = 0; j < meshes.size(); j++) {
                    Mesh * mesh = meshes[j];
                    renderer_->DrawGBuffer(mesh->opengl_object_, mesh->opengl_textures_, rendering_object->model_matrix_, mesh->mat_);
                }
            }

            break;
        }
        case RENDER_MODE::VIEW_FRUSTUM_CULLING:
        {
            FrustumG f;
            float * p = (float*)glm::value_ptr(camera_->GetProjectionMatrix());
            float * v = (float*)glm::value_ptr(camera_->GetViewMatrix());
            float m[16];
            multMat(m, v, p);
            f.setFrustum(m);

            size_t number_of_objects_to_draw_ = objects_to_draw_.size();
            for (size_t i = 0; i < number_of_objects_to_draw_; i++) {
                GraphicsObject * rendering_object = objects_to_draw_.at(i);
                rendering_object->SetModelMatrix();
                std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
                for (size_t i = 0; i < meshes.size(); i++) {
                    Mesh * mesh = meshes[i];

                    float x_offset = rendering_object->translation_matrix_[3][0];
                    float y_offset = rendering_object->translation_matrix_[3][1];
                    float z_offset = rendering_object->translation_matrix_[3][2];

                    opengl::OpenGLObject & gl_object = mesh->opengl_object_;
                    int ret = f.boxInFrustum(AABox(Vec3(gl_object.min_x_ + x_offset, gl_object.min_y_ + y_offset, gl_object.min_z_ + z_offset), gl_object.max_x_ - gl_object.min_x_, gl_object.max_y_ - gl_object.min_y_, gl_object.max_z_ - gl_object.min_z_));
                    if (ret != FrustumG::OUTSIDE) renderer_->DrawGBuffer(gl_object, mesh->opengl_textures_, rendering_object->model_matrix_, mesh->mat_);
                }
            }
            break;
        }
        case RENDER_MODE::OCCLUSION_QUERIES:
        {
            size_t number_of_objects_to_draw_ = objects_to_draw_.size();

            for (size_t i = 0; i < number_of_objects_to_draw_; i++) {
                GraphicsObject * rendering_object = objects_to_draw_.at(i);
                rendering_object->SetModelMatrix();
                std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
                for (size_t j = 0; j < meshes.size(); j++) {
                    Mesh * mesh = meshes[j];

                    GLint pixel_count = rendering_object->mesh_queries_[j].GetResult();
                    bool visible = pixel_count > 0;

                    rendering_object->mesh_queries_[j].Begin(GL_SAMPLES_PASSED);

                    if (visible) {
                        renderer_->EnableColorWriting(true);
                        renderer_->EnableDepthWriting(true);

                        renderer_->DrawGBuffer(mesh->opengl_object_, mesh->opengl_textures_, rendering_object->model_matrix_, mesh->mat_);

                    }
                    else {
                        renderer_->EnableColorWriting(false);
                        renderer_->EnableDepthWriting(false);

                        renderer_->DrawBoundingBox(mesh->opengl_object_, rendering_object->model_matrix_);
                    }

                    rendering_object->mesh_queries_[j].End();
                }
            }

            renderer_->EnableColorWriting(true);
            renderer_->EnableDepthWriting(true);

            break;
        }
        }

        renderer_->g_buffer_->UnBind();


        bool ssao = ConfigurationFile::instance().DoSSAO();     
        if (ssao) {
            ConsoleCommand command;
            /* Perform SSAO on the GBuffer */
            command = ConsoleParser::GetInstance().GetLastCommand();
            if (command.type_ == COMMAND_SSAO_SEPARABLE && separable_ao != static_cast<bool>(command.arg_1_)) {
                separable_ao = static_cast<bool>(command.arg_1_);
            }
            if (!separable_ao) {
                renderer_->frame_buffer_one_->Bind();
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                renderer_->DrawSSAO();
                renderer_->frame_buffer_one_->Unbind();
            } else {
                renderer_->frame_buffer_one_->Bind();
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                renderer_->DrawSeparableAO();
                renderer_->frame_buffer_one_->Unbind();
            }

            /* Do bluring or not? */
            command = ConsoleParser::GetInstance().GetLastCommand();
            if (command.type_ == COMMAND_SSAO_BLUR && ssao_blur != static_cast<bool>(command.arg_1_)) {
                ssao_blur = static_cast<bool>(command.arg_1_);
            }
            GLuint ssao_texture;
            if (ssao_blur == true) {
                renderer_->frame_buffer_two_->Bind();
                renderer_->BlurTexture(renderer_->frame_buffer_one_->output_texture_);
                renderer_->frame_buffer_two_->Unbind();

                ssao_texture = renderer_->frame_buffer_two_->output_texture_;
            } else {
                ssao_texture = renderer_->frame_buffer_one_->output_texture_;
            }

            /* What should I draw? */
            if (command.type_ == COMMAND_SSAO_DRAW_SSAO && draw_ssao_texture != static_cast<bool>(command.arg_1_)) {
                draw_ssao_texture = static_cast<bool>(command.arg_1_);
            }
            if (draw_ssao_texture) {
                renderer_->DrawTexture(ssao_texture, true);
            } else {
                size_t number_of_point_lights_ = point_lights_to_draw_.Items();
                renderer_->SetPointLightsNumber(number_of_point_lights_);
                for (size_t i = 0; i < number_of_point_lights_; i++) {
                    std::string index = std::to_string(i);

                    PointLight * light;
                    point_lights_to_draw_.Get(light);
                    renderer_->SetPointLight(index,
                        light->position_,
                        light->ambient_,
                        light->diffuse_,
                        light->specular_,
                        light->attenutation_.constant_,
                        light->attenutation_.linear_,
                        light->attenutation_.quadratic_
                    );
                }

                renderer_->DrawFinalPass(ssao_texture);
            }

        } else {

            size_t number_of_point_lights_ = point_lights_to_draw_.Items();
            renderer_->SetPointLightsNumber(number_of_point_lights_);
            for (size_t i = 0; i < number_of_point_lights_; i++) {
                std::string index = std::to_string(i);
            
                PointLight * light;
                point_lights_to_draw_.Get(light);
                renderer_->SetPointLight(index,
                    light->position_,
                    light->ambient_,
                    light->diffuse_,
                    light->specular_,
                    light->attenutation_.constant_,
                    light->attenutation_.linear_,
                    light->attenutation_.quadratic_
                );
            }
            
            renderer_->DrawFinalPass(renderer_->frame_buffer_one_->output_texture_);
        }

        std::string occlusion_text;
        if (frr_render_mode == RENDER_MODE::REGULAR) occlusion_text = "Regular";
        else if (frr_render_mode == RENDER_MODE::VIEW_FRUSTUM_CULLING) occlusion_text = "View frustum culling";
        else if (frr_render_mode == RENDER_MODE::OCCLUSION_QUERIES) occlusion_text = "Occlusion queries";

        renderer_->Draw2DText(occlusion_text, 0.0f, context_->GetWindowHeight() - 20, 0.5, glm::vec3(1, 0, 0));
        while(text_to_draw_.Items() > 0) {
            TEXT_DRAW_t text;
            text_to_draw_.Get(text);
            renderer_->Draw2DText(text.text_, text.x, text.y, text.scale, text.color);
        }

    }

}
}