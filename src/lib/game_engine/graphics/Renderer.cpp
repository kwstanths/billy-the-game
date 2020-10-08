#include "Renderer.hpp"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "game_engine/core/ErrorCodes.hpp"
#include "game_engine/core/ConfigurationFile.hpp"
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

        /* Init render queues */
        point_lights_to_draw_.Init(GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS);
        rendering_queues_ = std::vector<utility::CircularBuffer<MESH_DRAW_t>>(2);
        rendering_queues_[0].Init(GAME_ENGINE_RENDERER_MAX_OBJECTS);
        rendering_queues_[1].Init(GAME_ENGINE_RENDERER_MAX_OBJECTS);
        text_to_draw_.Init(512);

        instancing_.Init();

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
        
        renderer_->shadow_maps_->ClearDepth();
        renderer_->shadow_maps_->Unbind();

        renderer_->g_buffer_->Bind();
        // Light blue
        //renderer_->g_buffer_->ClearColor(0.0f / 255, 138.0 / 255, 145.0 / 255, 1);
        // Black
        renderer_->g_buffer_->ClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderer_->g_buffer_->UnBind();

        point_lights_to_draw_.Clear();
        rendering_queues_[0].Clear();
        rendering_queues_[1].Clear();
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

    void Renderer::SetSkybox(MaterialSkybox * skybox)
    {
        skybox_ = skybox;
        renderer_->skybox_ = skybox->texture_cubemap_;
    }

    void Renderer::SetLightShadows(DirectionalLight * light)
    {
        light_shadows_ = light;
    }

    int Renderer::Draw(GraphicsObject * rendering_object) {
        if (!rendering_object->IsInited()) return -1;

        /* Get the meshes of the object to draw, and put them in their respective queues */

        std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
        for (size_t j = 0; j < meshes.size(); j++) {
            Mesh * mesh = meshes[j];
            // If the material of  the mesh has been overriden, use the new, otherwise, use the base model material
            Material * material = (rendering_object->model_materials_[j] != nullptr) ? rendering_object->model_materials_[j] : rendering_object->model_->materials_[j];

            utility::CircularBuffer<MESH_DRAW_t>& queue = rendering_queues_[material->rendering_queue_];

            if (queue.IsFull()) {
                dt::Console(dt::CRITICAL, "Rendering queue: " + std::to_string(material->rendering_queue_) + "is full");
                return -1;
            }
            rendering_object->SetModelMatrix();
            queue.Push(MESH_DRAW_t(mesh, material, &rendering_object->model_matrix_, rendering_object->model_vbo_, 1));
        }

        return 0;
    }

    int Renderer::DrawTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color) {

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

    void Renderer::AddInstance(Material * material, Mesh * mesh, glm::mat4 & position)
    {
        instancing_.AddInstance(mesh, material, position);
    }

    int Renderer::SetCamera(gl::OpenGLCamera * camera) {
        
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        
        camera_ = camera;
        camera->Init(context_);

        return 0;
    }

    int Renderer::RenderGBuffer(MESH_DRAW_t& draw_call) {
        frr_render_mode = static_cast<RENDER_MODE>(ConfigurationFile::GetInstance().GetRenderingMethod());
        switch (frr_render_mode)
        {
        case RENDER_MODE::REGULAR:
        {
                Mesh * mesh = draw_call.mesh_;
                draw_call.material_->Render(renderer_, mesh->opengl_object_, draw_call.model_matrix_, draw_call.model_matrix_vbo_, draw_call.amount_);
                draw_calls_++;
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

            Mesh * mesh = draw_call.mesh_;
            glm::mat4& model_matrix = *draw_call.model_matrix_;

            /* Get object position */
            float x_offset = model_matrix[3][0];
            float y_offset = model_matrix[3][1];
            float z_offset = model_matrix[3][2];

            /* Get aabox */
            opengl::OpenGLObject & gl_object = mesh->opengl_object_;
            Vector3D min({ gl_object.min_x_ + x_offset, gl_object.min_y_ + y_offset, gl_object.min_z_ + z_offset });
            Vector3D max({ gl_object.max_x_ + x_offset, gl_object.max_y_ + y_offset, gl_object.max_z_ + z_offset });
            AABox<3> box(min, max);
            int ret = f.BoxInFrustum(box);
            if (ret != Frustum::OUTSIDE) {
                draw_call.material_->Render(renderer_, mesh->opengl_object_, draw_call.model_matrix_, draw_call.model_matrix_vbo_, draw_call.amount_);
                draw_calls_++;
            }

            break;
        }
        }

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

        if (!instancing_.buffers_prepared_) {
            instancing_.PrepareBuffers();
        }
        for (size_t i = 0; i < instancing_.instanced_draws_.size(); i++) {
            Instancing::InstanceDrawCall data = instancing_.instanced_draws_[i];
            rendering_queues_[data.material_->rendering_queue_].Push(MESH_DRAW_t(data.mesh_, data.material_, data.model_matrices_, data.model_matrices_buffer_, data.amount_));
        }

        draw_calls_ = 0;
        draw_calls_shadows_ = 0;

        /* Check if shadows are enabled or not */
        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_SHADOW_MAPPING) shadows = static_cast<bool>(command.arg_1_);
        
        renderer_->use_shadows_ = shadows;
        if (shadows && light_shadows_ != nullptr) {
            /* Calculate cascaded shadow maps matrices */
            renderer_->shadow_maps_->CalculateProjectionMatrices(light_shadows_->direction_, camera_);

            /* Render the shadow maps with all the GBuffer object */
            size_t n_of_cascades = renderer_->shadow_maps_->GetNCascades();
            for (size_t i = 0; i < n_of_cascades; i++) {
                renderer_->shadow_maps_->Bind(i);
                renderer_->shadow_maps_->ConfigureViewport();
                renderer_->EnableColorWriting(false);
                renderer_->EnableDepthWriting(true);
                renderer_->SetShadowMap(renderer_->shadow_maps_->GetLightspaceMatrix(i));

                //glCullFace(GL_FRONT);
                glDisable(GL_CULL_FACE);

                /* Iterate the gbuffer queue */
                utility::CircularBuffer<MESH_DRAW_t>& queue = rendering_queues_[0];
                for (utility::CircularBuffer<MESH_DRAW_t>::iterator itr = queue.begin(); itr != queue.end(); ++itr) {
                    MESH_DRAW_t& draw_call = *itr;
                    Mesh * mesh = draw_call.mesh_;

                    draw_call.material_->RenderShadow(renderer_, mesh->opengl_object_, draw_call.model_matrix_, draw_call.model_matrix_vbo_, draw_call.amount_);
                    draw_calls_shadows_++;
                }

                //glCullFace(GL_BACK);
                glEnable(GL_CULL_FACE);

            }
            renderer_->shadow_maps_->Unbind();
            renderer_->EnableColorWriting(true);
        }

        command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_WIREFRAME && !math::Equal(draw_wireframe_, static_cast<bool>(command.arg_1_))) {
            draw_wireframe_ = static_cast<bool>(command.arg_1_);
        }
        renderer_->DrawWireframe(draw_wireframe_);
        

        /* Render GBuffer */
        glViewport(0, 0, context_->GetWindowWidth(), context_->GetWindowHeight());
        /* Bind the gbuffer, and draw the geometry */
        renderer_->g_buffer_->Bind();
        utility::CircularBuffer<MESH_DRAW_t>& queue = rendering_queues_[0];
        for (utility::CircularBuffer<MESH_DRAW_t>::iterator itr = queue.begin(); itr != queue.end(); ++itr) {
            MESH_DRAW_t& draw_call = *itr;
            RenderGBuffer(draw_call);
        }
        renderer_->g_buffer_->UnBind();
        renderer_->DrawWireframe(false);
        
        // Post processing stack should go here
        // Is AO enabled?
        bool ssao = ConfigurationFile::GetInstance().DoSSAO();
        if (ssao) {
            ConsoleCommand command;
            /* Perform SSAO on the GBuffer */
            /* Perform classic or separable AO? */
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
                /* Draw final scene, set point lights in the scene */
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
            /* If not AO, perform final pass directly */

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
            draw_calls_++;
        }
        
        
        /* Render forward queue */
        renderer_->DrawWireframe(draw_wireframe_);
        queue = rendering_queues_[1];
        for (utility::CircularBuffer<MESH_DRAW_t>::iterator itr = queue.begin(); itr != queue.end(); ++itr) {
            MESH_DRAW_t& draw_call = *itr;
            Mesh * mesh = draw_call.mesh_;

            draw_call.material_->Render(renderer_, mesh->opengl_object_, draw_call.model_matrix_, draw_call.model_matrix_vbo_, draw_call.amount_);
            draw_calls_++;
        }
        renderer_->DrawWireframe(false);


        /* Render the skybox */
        //if (skybox_ != nullptr) renderer_->DrawSkybox(skybox_->texture_cubemap_);

        /* Render overlay */
        while (text_to_draw_.Items() > 0) {
            TEXT_DRAW_t text;
            text_to_draw_.Get(text);
            renderer_->Draw2DText(text.text_, text.x, text.y, text.scale, text.color);
            draw_calls_++;
        }
        renderer_->Draw2DText("Draw calls: " + std::to_string(draw_calls_), 0.0f, context_->GetWindowHeight() - 50, 0.5, glm::vec3(1, 0, 0));
        renderer_->Draw2DText("Shadow draw calls: " + std::to_string(draw_calls_shadows_) , 0.0f, context_->GetWindowHeight() - 80, 0.5, glm::vec3(1, 0, 0));
    }

}
}