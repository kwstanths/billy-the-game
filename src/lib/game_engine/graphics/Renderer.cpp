#include "Renderer.hpp"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "game_engine/ErrorCodes.hpp"
#include "game_engine/ConfigurationFile.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/math/Matrices.hpp"
#include "opengl/OpenGLIncludes.hpp"

#include "game_engine/graphics/Frustum.hpp"

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
        gbuffer_objects_.Init(GAME_ENGINE_RENDERER_MAX_OBJECTS);
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
        
        renderer_->shadow_map_->Bind();
        renderer_->shadow_map_->ClearDepth();
        renderer_->shadow_map_->Unbind();

        renderer_->g_buffer_->Bind();
        renderer_->g_buffer_->ClearColor(0.0f / 255, 138.0 / 255, 145.0 / 255, 1);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderer_->g_buffer_->UnBind();

        point_lights_to_draw_.Clear();
        gbuffer_objects_.Clear();
        text_to_draw_.Clear();
    }

    void Renderer::EndFrame() {
        /* Set camera parameters */
        Renderer::SetView();

        /* Flush draw calls */
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
        if (!rendering_object->IsInited()) return -1;

        if (gbuffer_objects_.IsFull()) {
            dt::Console(dt::CRITICAL, "GBuffer objects vector full");
            return -1;
        }
        gbuffer_objects_.Push(rendering_object);
        rendering_object->SetModelMatrix();

        return 0;
    }

    int Renderer::DrawTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color) {

        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_SHADOW_MAPPING) shadows = static_cast<bool>(command.arg_1_);

        if (shadows) {

            renderer_->shadow_map_->ConfigureViewport();
            renderer_->shadow_map_->Bind();

            glDisable(GL_CULL_FACE);

            opengl::OpenGLTriangle t;
            t.Init(v1, v2, v3);

            renderer_->DrawShadowMap(t, glm::mat4(1.0f));

            glEnable(GL_CULL_FACE);

            renderer_->shadow_map_->Unbind();

            t.Destroy();
        }

        glViewport(0, 0, context_->GetWindowWidth(), context_->GetWindowHeight());

        renderer_->g_buffer_->Bind();
        renderer_->DrawGBufferTriangle(v1, v2, v3, color);
        renderer_->g_buffer_->UnBind();

        return 0;
    }

    int Renderer::DrawLine(math::Vector3D start, math::Vector3D stop, float size, glm::vec3 color) {

        renderer_->g_buffer_->Bind();
        renderer_->DrawLine(start.ToGlm(), stop.ToGlm(), color);
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

    void Renderer::SetShadowMap(glm::mat4 & light_view_matrix, glm::mat4 & light_projection_matrix) {
        renderer_->SetShadowMap(light_view_matrix, light_projection_matrix);
    }

    int Renderer::SetCamera(gl::OpenGLCamera * camera) {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        camera_ = camera;
        camera->Init(context_);

        return 0;
    }

    int Renderer::RenderGBuffer(GraphicsObject * rendering_object) {

        glViewport(0, 0, context_->GetWindowWidth(), context_->GetWindowHeight());

        frr_render_mode = static_cast<RENDER_MODE>(ConfigurationFile::instance().GetRenderingMethod());

        /* Bind the gbuffer, and draw the geometry */
        renderer_->g_buffer_->Bind();

        switch (frr_render_mode)
        {
        case RENDER_MODE::REGULAR:
        {
            std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
            for (size_t j = 0; j < meshes.size(); j++) {
                Mesh * mesh = meshes[j];
                renderer_->DrawGBuffer(mesh->opengl_object_, mesh->opengl_textures_, rendering_object->model_matrix_, mesh->mat_);
            }

            break;
        }
        case RENDER_MODE::VIEW_FRUSTUM_CULLING:
        {
            /* Initialize camera frustum */
            Frustum f;
            float * p = (float*)glm::value_ptr(camera_->GetProjectionMatrix());
            float * v = (float*)glm::value_ptr(camera_->GetViewMatrix());
            float m[16];
            MultMat(m, v, p);
            f.SetFrustum(m);

            /* Render meshes */
            std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
            for (size_t i = 0; i < meshes.size(); i++) {
                Mesh * mesh = meshes[i];

                float x_offset = rendering_object->translation_matrix_[3][0];
                float y_offset = rendering_object->translation_matrix_[3][1];
                float z_offset = rendering_object->translation_matrix_[3][2];

                opengl::OpenGLObject & gl_object = mesh->opengl_object_;
                Vector3D min({ gl_object.min_x_ + x_offset, gl_object.min_y_ + y_offset, gl_object.min_z_ + z_offset });
                Vector3D max({ gl_object.max_x_ + x_offset, gl_object.max_y_ + y_offset, gl_object.max_z_ + z_offset });
                AABox<3> box(min, max);
                int ret = f.BoxInFrustum(box);
                if (ret != Frustum::OUTSIDE) renderer_->DrawGBuffer(gl_object, mesh->opengl_textures_, rendering_object->model_matrix_, mesh->mat_);
            }

            break;
        }
        case RENDER_MODE::OCCLUSION_QUERIES:
        {

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

            renderer_->EnableColorWriting(true);
            renderer_->EnableDepthWriting(true);

            break;
        }
        }

        renderer_->g_buffer_->UnBind();

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

        //glDisable(GL_CULL_FACE);
        //glBlendFunc(GL_ONE, GL_ONE);

        /* Check if shadows are enabled or not */
        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_SHADOW_MAPPING) shadows = static_cast<bool>(command.arg_1_);

        // Render shadow map for all objects in the gbuffer queue
        if (shadows) {

            renderer_->shadow_map_->ConfigureViewport();
            renderer_->shadow_map_->Bind();

            //glCullFace(GL_FRONT);
            glDisable(GL_CULL_FACE);

            for (utility::CircularBuffer<GraphicsObject *>::iterator itr = gbuffer_objects_.begin(); itr != gbuffer_objects_.end(); ++itr) {
                GraphicsObject * rendering_object = *itr;

                std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
                for (size_t j = 0; j < meshes.size(); j++) {
                    Mesh * mesh = meshes[j];
                    renderer_->DrawShadowMap(mesh->opengl_object_, rendering_object->model_matrix_);
                }
            }

            //glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);

            renderer_->shadow_map_->Unbind();
        }

        // Render GBuffer
        for (utility::CircularBuffer<GraphicsObject *>::iterator itr = gbuffer_objects_.begin(); itr != gbuffer_objects_.end(); ++itr) {
            GraphicsObject * rendering_object = *itr;
            RenderGBuffer(rendering_object);
        }

        // Post processing stack should go here
        // Is AO enabled?
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
            }
            else {
                renderer_->frame_buffer_one_->Bind();
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                renderer_->DrawSeparableAO();
                renderer_->frame_buffer_one_->Unbind();
            }

            /* Do AO bluring or not? */
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
            }
            else {
                ssao_texture = renderer_->frame_buffer_one_->output_texture_;
            }

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            /* What should I draw? the AO texture for debugging, or the final scene? */
            if (command.type_ == COMMAND_SSAO_DRAW_SSAO && draw_ssao_texture != static_cast<bool>(command.arg_1_)) {
                draw_ssao_texture = static_cast<bool>(command.arg_1_);
            }
            if (draw_ssao_texture) {
                renderer_->DrawTexture(ssao_texture, true);
            }
            else {
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
        }
        else {
            /* Clear the color of frame buffer to one, since this texture will be used as the ambient color texture */
            renderer_->frame_buffer_one_->ClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

        // Render text with forward rendering
        std::string occlusion_text;
        if (frr_render_mode == RENDER_MODE::REGULAR) occlusion_text = "Regular";
        else if (frr_render_mode == RENDER_MODE::VIEW_FRUSTUM_CULLING) occlusion_text = "View frustum culling";
        else if (frr_render_mode == RENDER_MODE::OCCLUSION_QUERIES) occlusion_text = "Occlusion queries";
        renderer_->Draw2DText(occlusion_text, 0.0f, context_->GetWindowHeight() - 20, 0.5, glm::vec3(1, 0, 0));

        while (text_to_draw_.Items() > 0) {
            TEXT_DRAW_t text;
            text_to_draw_.Get(text);
            renderer_->Draw2DText(text.text_, text.x, text.y, text.scale, text.color);
        }

    }

}
}