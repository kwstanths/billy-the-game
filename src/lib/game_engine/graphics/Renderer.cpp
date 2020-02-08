#include "Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "cal3d/renderer.h"

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/ErrorCodes.hpp"
#include "game_engine/ConfigurationFile.hpp"
#include "game_engine/graphics/Frustum.hpp"
#include "game_engine/graphics/opengl/OpenGLIncludes.hpp"


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
        
        renderer_->shadow_map_->Bind();
        renderer_->shadow_map_->ClearDepth();
        renderer_->shadow_map_->Unbind();

        renderer_->g_buffer_->Bind();
        renderer_->g_buffer_->ClearColor(0.0f / 255, 138.0 / 255, 145.0 / 255, 1);
        glClear(GL_DEPTH_BUFFER_BIT);
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

        if (!rendering_object->IsInited()) return -1;

        if (rendering_object->type_ == GraphicsObject::ASSIMP) {
            ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
            if (command.type_ == COMMAND_SHADOW_MAPPING) shadows = static_cast<bool>(command.arg_1_);

            rendering_object->SetModelMatrix();
            if (shadows) {

                renderer_->shadow_map_->ConfigureViewport();
                renderer_->shadow_map_->Bind();

                //glCullFace(GL_FRONT);
                glDisable(GL_CULL_FACE);
                std::vector<Mesh *>& meshes = rendering_object->model_->meshes_;
                for (size_t j = 0; j < meshes.size(); j++) {
                    Mesh * mesh = meshes[j];
                    renderer_->DrawShadowMap(mesh->opengl_object_, rendering_object->model_matrix_);
                }

                //glCullFace(GL_BACK);
                glEnable(GL_CULL_FACE);

                renderer_->shadow_map_->Unbind();
            }

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

            return 0;
        } else if (rendering_object->type_ == GraphicsObject::ANIMATED) {
            
            rendering_object->SetModelMatrix();
            
            // get the renderer of the model
            cal3d::CalRenderer *pCalRenderer;
            pCalRenderer = rendering_object->m_calModel->getRenderer();

            // begin the rendering loop
            if (!pCalRenderer->beginRendering()) return -1;

            // get the number of meshes
            int meshCount;
            meshCount = pCalRenderer->getMeshCount();

            size_t mesh_index = 0;
            // render all meshes of the model
            int meshId;
            for (meshId = 0; meshId < meshCount; meshId++)
            {
                // get the number of submeshes
                int submeshCount;
                submeshCount = pCalRenderer->getSubmeshCount(meshId);

                // render all submeshes of the mesh
                int submeshId;
                for (submeshId = 0; submeshId < submeshCount; submeshId++)
                {
                    // select mesh and submesh for further data access
                    if (pCalRenderer->selectMeshSubmesh(meshId, submeshId))
                    {
                        Material_t mtl;
                        unsigned char meshColor[4];
                        GLfloat materialColor[4];

                        // set the material ambient color
                        pCalRenderer->getAmbientColor(&meshColor[0]);
                        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
                        mtl.ambient_ = glm::vec3(materialColor[0], materialColor[1], materialColor[2]);

                        // set the material diffuse color
                        pCalRenderer->getDiffuseColor(&meshColor[0]);
                        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
                        mtl.diffuse_ = glm::vec3(materialColor[0], materialColor[1], materialColor[2]);

                        // set the material specular color
                        pCalRenderer->getSpecularColor(&meshColor[0]);
                        materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
                        mtl.specular_ = glm::vec3(materialColor[0], materialColor[1], materialColor[2]);

                        // set the material shininess factor
                        float shininess = pCalRenderer->getShininess();
                        mtl.shininess_ = shininess;

                        // get the transformed vertices of the submesh
                        static float meshVertices[30000][3];
                        int vertexCount;
                        vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

                        // get the transformed normals of the submesh
                        static float meshNormals[30000][3];
                        pCalRenderer->getNormals(&meshNormals[0][0]);

                        // get the texture coordinates of the submesh
                        static float meshTextureCoordinates[30000][2];
                        int textureCoordinateCount;
                        textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

                        std::vector<Vertex_t> vertices;
                        for (size_t vertex = 0; vertex < vertexCount; vertex++) {
                            vertices.push_back(Vertex_t(glm::vec3(meshVertices[vertex][0], meshVertices[vertex][1], meshVertices[vertex][2]),
                                glm::vec3(meshNormals[vertex][0], meshNormals[vertex][1], meshNormals[vertex][2]),
                                glm::vec2(meshTextureCoordinates[vertex][0], meshTextureCoordinates[vertex][1])));
                        }
                        opengl::OpenGLObject * temp = rendering_object->objects_[mesh_index++];
                        temp->SetVertices(vertices);

                        if (shadows) {

                            renderer_->shadow_map_->ConfigureViewport();
                            renderer_->shadow_map_->Bind();

                            glCullFace(GL_FRONT);
                            renderer_->DrawShadowMap(*temp, rendering_object->model_matrix_);
                            glCullFace(GL_BACK);

                            renderer_->shadow_map_->Unbind();
                        }

                        glViewport(0, 0, context_->GetWindowWidth(), context_->GetWindowHeight());

                        /* Bind the gbuffer, and draw the geometry */
                        renderer_->g_buffer_->Bind();
                        renderer_->DrawGBuffer(*temp, rendering_object->textures_, rendering_object->model_matrix_, mtl);
                        renderer_->g_buffer_->UnBind();
                    }
                }
            }

            // end the rendering
            pCalRenderer->endRendering();
        }

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

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

        /* This piece of code writes the render mode on the top left of the screen */
        /*
        std::string occlusion_text;
        if (frr_render_mode == RENDER_MODE::REGULAR) occlusion_text = "Regular";
        else if (frr_render_mode == RENDER_MODE::VIEW_FRUSTUM_CULLING) occlusion_text = "View frustum culling";
        else if (frr_render_mode == RENDER_MODE::OCCLUSION_QUERIES) occlusion_text = "Occlusion queries";
        renderer_->Draw2DText(occlusion_text, 0.0f, context_->GetWindowHeight() - 20, 0.5, glm::vec3(1, 0, 0));
        */

        while(text_to_draw_.Items() > 0) {
            TEXT_DRAW_t text;
            text_to_draw_.Get(text);
            renderer_->Draw2DText(text.text_, text.x, text.y, text.scale, text.color);
        }

    }

}
}