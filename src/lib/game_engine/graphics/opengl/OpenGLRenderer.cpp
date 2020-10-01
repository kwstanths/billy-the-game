#include "OpenGLRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "game_engine/core/ConsoleParser.hpp"
#include "game_engine/math/RNG.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"
#include "game_engine/graphics/AssetManager.hpp"
#include "game_engine/core/FileSystem.hpp"

#include "debug_tools/Console.hpp"

namespace math = game_engine::math;


namespace game_engine { namespace graphics { namespace opengl {

    OpenGLRenderer::OpenGLRenderer() {
    
        is_inited_ = false;
        text_renderer_ = new OpenGLText();
        g_buffer_ = new OpenGLGBuffer();
        frame_buffer_one_ = new OpenGLFrameBufferTexture();
        frame_buffer_two_ = new OpenGLFrameBufferTexture();
        shadow_map_ = new OpenGLShadowMap();
    
    }
    
    int OpenGLRenderer::Init(OpenGLContext * context) {
        context_ = context;
    
        /* Initialize font */
        std::string font_location = context->GetFontLocation();
        if (text_renderer_->Init(font_location, context) != 0)
            debug_tools::ConsoleInfoL(debug_tools::CRITICAL, "Font not found", "name", font_location);
    
        /* Initialize shader variables */
        shader_vertices_color_ = context_->shader_vertices_color_;
    
        {
            /* Used to draw textures */
            shader_quad_ = context_->shader_quad_;
            shader_quad_.Use();
            shader_quad_.SetUniformInt(shader_quad_.uni_sampler_texture_, 0);
        }
    
        {
            /* The G buffer shader */
            shader_gbuffer_ = context_->shader_gbuffer_;
            shader_gbuffer_.Use();
            shader_gbuffer_.SetUniformInt(shader_gbuffer_.GetUniformLocation("object_material.texture_diffuse"), 0);
            shader_gbuffer_.SetUniformInt(shader_gbuffer_.GetUniformLocation("object_material.texture_specular"), 1);
        }

        {
            shader_standard_ = context_->shader_standard_;
            shader_standard_.Use();
            shader_standard_.SetUniformInt(shader_standard_.GetUniformLocation("object_material.texture_diffuse"), 0);
            shader_standard_.SetUniformInt(shader_standard_.GetUniformLocation("object_material.texture_specular"), 1);
        }
    
        {
            /* The shadow map shader */
            shader_shadow_map_ = context_->shader_shadow_map_;
        }
    
        {
            /* The ssao shader */
            shader_ssao_ = context_->shader_ssao_;
            shader_ssao_.Use();
            shader_ssao_.SetUniformInt(shader_ssao_.uni_g_position_, 0);
            shader_ssao_.SetUniformInt(shader_ssao_.uni_g_normal_, 1);
            shader_ssao_.SetUniformInt(shader_ssao_.uni_noise_texture_, 2);
    
            ssao_radius_used_ = 2.0;
            shader_ssao_.SetUniformFloat(shader_ssao_.GetUniformLocation(shader_ssao_radius), ssao_radius_used_);
    
            ssao_samples_used_ = 64;
            shader_ssao_.SetUniformInt(shader_ssao_.GetUniformLocation(shader_ssao_samples_size), ssao_samples_used_);
    
            /* Generate random samples inside a sphere for ssao */
            number_of_samples_ = 128;
            random_samples_kernel_ = std::vector<glm::vec3>(number_of_samples_);
            game_engine::math::MersenneTwisterGenerator rng(game_engine::math::MersenneTwisterGenerator::SEVEN);
            for (size_t i = 0; i < number_of_samples_; i++) {
                random_samples_kernel_[i] = glm::vec3(rng.rng_between(-1, 1), rng.rng_between(-1, 1), rng.rng_between(-1, 1));
                glm::normalize(random_samples_kernel_[i]);
    
                /* Scale the samples so that we have more closer to (0,0,0) */
                float scale = float(i) / float(number_of_samples_);
                scale = math::Lerp(0.1f, 1.0f, scale * scale);
                random_samples_kernel_[i] *= scale;
            }
    
            /* Set the samples to the ssao shader */
            for (unsigned int i = 0; i < number_of_samples_; i++)
                shader_ssao_.SetUniformVec3(shader_ssao_.GetUniformLocation("samples[" + std::to_string(i) + "]"), random_samples_kernel_[i]);
    
            /* Create the noise texture */
            noise_size_ = 25;
            noise_ = std::vector<glm::vec3>(noise_size_);
            for (size_t i = 0; i < noise_size_; i++) {
                noise_[i] = glm::vec3(rng.rng_between(-1, 1), rng.rng_between(-1, 1), rng.rng_between(-1, 1));
                glm::normalize(noise_[i]);
            }
    
            glGenTextures(1, &noise_texture_);
            glBindTexture(GL_TEXTURE_2D, noise_texture_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sqrt(noise_size_), sqrt(noise_size_), 0, GL_RGB, GL_FLOAT, &noise_[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
            /* SSAO intensity */
            ssao_intensity_ = 1.0f;
            shader_ssao_.SetUniformFloat(shader_ssao_.uni_intensity_, ssao_intensity_);
    
            /* SSAO bias */
            ssao_bias_ = 0.0625f;
            shader_ssao_.SetUniformFloat(shader_ssao_.uni_bias_, ssao_bias_);
        }
    
        {
            /* The separable ao shader */
            shader_separable_ao_ = context_->shader_separable_ao_;
            shader_separable_ao_.Use();
            shader_separable_ao_.SetUniformInt(shader_separable_ao_.uni_g_position_, 0);
            shader_separable_ao_.SetUniformInt(shader_separable_ao_.uni_g_normal_, 1);
            shader_separable_ao_.SetUniformInt(shader_separable_ao_.uni_noise_texture_, 2);
            shader_separable_ao_.SetUniformFloat(shader_separable_ao_.GetUniformLocation(shader_ssao_radius), ssao_radius_used_);
    
            separable_ao_samples_used_ = 14;
            shader_separable_ao_.SetUniformInt(shader_separable_ao_.GetUniformLocation(shader_ssao_samples_size), separable_ao_samples_used_);
    
            /* Set the samples to the separable ao shader */
            for (unsigned int i = 0; i < number_of_samples_; i++)
                shader_separable_ao_.SetUniformVec3(shader_separable_ao_.GetUniformLocation("samples[" + std::to_string(i) + "]"), random_samples_kernel_[i]);
    
            /* separable ao intensity */
            shader_separable_ao_.SetUniformFloat(shader_separable_ao_.uni_intensity_, ssao_intensity_);
            shader_ssao_.SetUniformFloat(shader_ssao_.uni_bias_, ssao_bias_);
        }
    
        /* The bluring shader */
        {
            shader_blur_ = context_->shader_blur_;
            shader_blur_.Use();
            shader_blur_.SetUniformInt(shader_blur_.GetUniformLocation(shader_sampler_texture), 0);
            blur_kernel_size_ = 5;
            shader_blur_.SetUniformInt(shader_blur_.GetUniformLocation(shader_blur_kernel_size), blur_kernel_size_);
        }
    
        {
            /* The final pass shader */
            shader_final_pass_ = context_->shader_final_pass_;
            shader_final_pass_.Use();
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_texture_gbuffer_position_, 0);
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_texture_gbuffer_normal_, 1);
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_texture_gbuffer_albedo_spec_, 2);
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_texture_ssao_, 3);
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_shadow_map_0_, 4);
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_shadow_map_1_, 5);
            shader_final_pass_.SetUniformInt(shader_final_pass_.uni_shadow_map_2_, 6);


        }

        {
            shader_draw_normals_ = context->shader_draw_normals_;
        }

        {
            shader_displacement_ = context->shader_displacement_;
            shader_displacement_.Use();
            shader_displacement_.SetUniformInt(shader_displacement_.uni_displacement_map_, 0);
            shader_displacement_.SetUniformInt(shader_displacement_.uni_texture_diffuse_, 1);
        }

        {
            shader_displacement_draw_normals_ = context->shader_displacement_draw_normals_;
            shader_displacement_draw_normals_.Use();
            shader_displacement_draw_normals_.SetUniformInt(shader_displacement_draw_normals_.uni_displacement_map_, 0);
        }

        {
            shader_water_ = context->shader_water_;
            shader_water_.Use();
            shader_water_.SetUniformInt(shader_water_.GetUniformLocation("object_material.texture_diffuse"), 0);
            shader_water_.SetUniformInt(shader_water_.GetUniformLocation("object_material.texture_specular"), 1);
            shader_water_.SetUniformInt(shader_water_.uni_texture_bump_, 2);
            shader_water_.SetUniformInt(shader_water_.uni_texture_depth_, 3);
            shader_water_.SetUniformInt(shader_water_.uni_texture_cubemap_, 4);
        }

        {
            shader_skybox_ = context->shader_skybox_;
            shader_skybox_.Use();
            shader_skybox_.SetUniformInt(shader_skybox_.uni_texture_cubemap_, 0);
        }

        /* Init GBuffer */
        g_buffer_->Init(context_);
        /* Init other frame buffers */
        frame_buffer_one_->Init(context_, GL_RED);
        frame_buffer_two_->Init(context_, GL_RED);
        /* Init the shadow map */
        shadow_map_->Init(1248, 1248);
    
        /* Initialize a quad geometry */
        {
            float quadVertices[] = {
                // vertex positions        // vertex uv coordinates
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
    
            /* Setup plane VAO */
            glGenVertexArrays(1, &VAO_Quad_);
            glGenBuffers(1, &VBO_Quad_);
            glBindVertexArray(VAO_Quad_);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_Quad_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
            glBindVertexArray(0);
        }
    
        skybox_cube_ = new OpenGLSkyboxCube();
        skybox_cube_->Init();

        /* Initialize an empty texture, used in several places */
        AssetManager& instance = AssetManager::GetInstance();
        texture_empty_ = instance.GetTexture(FileSystem::GetInstance().GetDirectoryAssets() + "/textures/spec_map_empty.png", GAME_ENGINE_TEXTURE_TYPE_EMPTY);
   
        is_inited_ = true;
        return 0;
    }

    int OpenGLRenderer::Destroy() {
    
        /* TODO Deallocate everything */
    
        glDeleteBuffers(1, &VBO_Quad_);
        glDeleteVertexArrays(1, &VAO_Quad_);
    
        is_inited_ = false;
        return 0;
    }
    
    bool OpenGLRenderer::IsInited() {
        return is_inited_;
    }
    
    void OpenGLRenderer::EnableColorWriting(bool enable) {
    
        glColorMask(enable, enable, enable, enable);
    }
    
    void OpenGLRenderer::EnableDepthWriting(bool enable) {
    
        glDepthMask(enable);
    }

    void OpenGLRenderer::DrawWireframe(bool enable)
    {
        if (enable) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    
    void OpenGLRenderer::SetShadowMap(glm::mat4& matrix_lightspace) {
        shader_shadow_map_.Use();
        shader_shadow_map_.SetUniformMat4(shader_shadow_map_.uni_Lightspace_, matrix_lightspace);
    }
    
    void OpenGLRenderer::SetView(OpenGLCamera * camera) {
    
        camera_ = camera;
        camera->CalculateView();
        
        shader_vertices_color_.Use();
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_View_, camera->view_matrix_);
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Projection_, camera->projection_matrix_);
    
        shader_gbuffer_.Use();
        shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_View_, camera->view_matrix_);
        shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_Projection_, camera->projection_matrix_);

        shader_standard_.Use();
        shader_standard_.SetUniformMat4(shader_standard_.uni_View_, camera->view_matrix_);
        shader_standard_.SetUniformMat4(shader_standard_.uni_Projection_, camera->projection_matrix_);
    
        shader_ssao_.Use();
        shader_ssao_.SetUniformMat4(shader_ssao_.uni_matrix_projection_, camera->projection_matrix_);
    
        shader_separable_ao_.Use();
        shader_separable_ao_.SetUniformMat4(shader_separable_ao_.uni_matrix_projection_, camera->projection_matrix_);
    
        shader_final_pass_.Use();
        shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_view_, camera->view_matrix_);
        shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_view_inverse_, camera->inverse_view_matrix_);
        shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_projection_, camera->projection_matrix_);

        shader_draw_normals_.Use();
        shader_draw_normals_.SetUniformMat4(shader_draw_normals_.uni_View_, camera->view_matrix_);
        shader_draw_normals_.SetUniformMat4(shader_draw_normals_.uni_Projection_, camera->projection_matrix_);

        shader_displacement_.Use();
        shader_displacement_.SetUniformMat4(shader_displacement_.uni_View_, camera->view_matrix_);
        shader_displacement_.SetUniformMat4(shader_displacement_.uni_Projection_, camera->projection_matrix_);

        shader_displacement_draw_normals_.Use();
        shader_displacement_draw_normals_.SetUniformMat4(shader_displacement_draw_normals_.uni_View_, camera->view_matrix_);
        shader_displacement_draw_normals_.SetUniformMat4(shader_displacement_draw_normals_.uni_Projection_, camera->projection_matrix_);

        shader_water_.Use();
        shader_water_.SetUniformMat4(shader_water_.uni_View_, camera->view_matrix_);
        shader_water_.SetUniformMat4(shader_water_.uni_Projection_, camera->projection_matrix_);

        /* Ignore translation of view matrix */
        glm::mat4 view = glm::mat4(glm::mat3(camera->view_matrix_));
        shader_skybox_.Use();
        shader_skybox_.SetUniformMat4(shader_skybox_.uni_View_, view);
        shader_skybox_.SetUniformMat4(shader_skybox_.uni_Projection_, camera->projection_matrix_);
    }
    
    int OpenGLRenderer::DrawGBufferStandard(OpenGLObject & object, glm::mat4 model, glm::vec3 diffuse, glm::vec3 specular, OpenGLTexture * diffuse_texture, OpenGLTexture * specular_texture) {
    
        if (!is_inited_) return -1;
        if (!object.IsInited()) return -1;

        /* TODO commented components not used */
        shader_gbuffer_.Use();
        /* Set the model uniform */
        shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_Model_, model);
        //shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.ambient"), mtl.ambient_);
        shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.diffuse"), diffuse);
        shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.specular"), specular);
        //shader_gbuffer_.SetUniformFloat(shader_gbuffer_.GetUniformLocation("object_material.shininess"), mtl.shininess_);
    
        glBindVertexArray(object.VAO_);
    
        object.SetupAttributes(&shader_gbuffer_);
    
        diffuse_texture->ActivateTexture(0);
        specular_texture->ActivateTexture(1);
    
        object.Render();
    
        /* Unbind */
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    
        return 0;
    }

    int OpenGLRenderer::DrawGBufferDisplacement(OpenGLObject & object, glm::mat4 & model, float specular_intensity, OpenGLTexture * displacement_texture, float displacement_mult, OpenGLTexture * diffuse_texture)
    {
        glBindVertexArray(object.VAO_);
        glPatchParameteri(GL_PATCH_VERTICES, 3);

        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_CONSTANT_TESSELLATION && !math::Equal(constant_tessellation_, static_cast<bool>(command.arg_1_))) {
            constant_tessellation_ = static_cast<bool>(command.arg_1_);
        }

        glm::vec3 camera_position;
        camera_->GetPositionVector(camera_position.x, camera_position.y, camera_position.z);

        shader_displacement_.Use();
        shader_displacement_.SetUniformMat4(shader_displacement_.uni_Model_, model);
        shader_displacement_.SetUniformVec3(shader_displacement_.uni_camera_world_position_, camera_position);
        shader_displacement_.SetUniformFloat(shader_displacement_.uni_displacement_intensity_, displacement_mult);
        shader_displacement_.SetUniformBool(shader_displacement_.uni_constant_tessellation_, constant_tessellation_);
        shader_displacement_.SetUniformFloat(shader_displacement_.uni_specular_intensity_, specular_intensity);


        object.SetupAttributes(&shader_displacement_);

        displacement_texture->ActivateTexture(0);
        diffuse_texture->ActivateTexture(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.element_buffer_);
        glDrawElements(GL_PATCHES, object.total_indices_, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        return 0;
    }

    int OpenGLRenderer::DrawDisplacementNormals(OpenGLObject & object, glm::mat4 & model, OpenGLTexture * displacement_texture, float displacement_mult, glm::vec3 color)
    {
        glBindVertexArray(object.VAO_);
        glPatchParameteri(GL_PATCH_VERTICES, 3);

        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_CONSTANT_TESSELLATION && !math::Equal(constant_tessellation_, static_cast<bool>(command.arg_1_))) {
            constant_tessellation_ = static_cast<bool>(command.arg_1_);
        }

        glm::vec3 camera_position;
        camera_->GetPositionVector(camera_position.x, camera_position.y, camera_position.z);

        shader_displacement_draw_normals_.Use();
        shader_displacement_draw_normals_.SetUniformMat4(shader_displacement_draw_normals_.uni_Model_, model);
        shader_displacement_draw_normals_.SetUniformVec3(shader_displacement_draw_normals_.uni_camera_world_position_, camera_position);
        shader_displacement_draw_normals_.SetUniformFloat(shader_displacement_draw_normals_.uni_displacement_intensity_, displacement_mult);
        shader_displacement_draw_normals_.SetUniformVec3(shader_displacement_draw_normals_.uni_color_, color);
        shader_displacement_draw_normals_.SetUniformBool(shader_displacement_draw_normals_.uni_constant_tessellation_, constant_tessellation_);

        object.SetupAttributes(&shader_displacement_);

        displacement_texture->ActivateTexture(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.element_buffer_);
        glDrawElements(GL_PATCHES, object.total_indices_, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        return 0;
    }

    int OpenGLRenderer::DrawStandard(OpenGLObject & object, glm::mat4 model, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, OpenGLTexture * diffuse_texture, OpenGLTexture * specular_texture)
    {

        if (!is_inited_) return -1;
        if (!object.IsInited()) return -1;

        /* TODO commented components not used */
        shader_standard_.Use();
        /* Set the model uniform */
        shader_standard_.SetUniformMat4(shader_standard_.uni_Model_, model);
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("object_material.ambient"), ambient);
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("object_material.diffuse"), diffuse);
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("object_material.specular"), specular);
        shader_standard_.SetUniformFloat(shader_standard_.GetUniformLocation("object_material.shininess"), shininess);

        glBindVertexArray(object.VAO_);

        object.SetupAttributes(&shader_gbuffer_);

        diffuse_texture->ActivateTexture(0);
        specular_texture->ActivateTexture(1);

        object.Render();

        /* Unbind */
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        return 0;
    }

    int OpenGLRenderer::DrawWater(OpenGLObject & object, glm::mat4 model, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, OpenGLTexture * diffuse_texture, OpenGLTexture * specular_texture, OpenGLTexture * bump_texture, std::vector<Wave_t>& waves)
    {
        glBindVertexArray(object.VAO_);
        glPatchParameteri(GL_PATCH_VERTICES, 3);

        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_CONSTANT_TESSELLATION && !math::Equal(constant_tessellation_, static_cast<bool>(command.arg_1_))) {
            constant_tessellation_ = static_cast<bool>(command.arg_1_);
        }
        else if (command.type_ == COMMAND_WATER_REFLECTANCE && !math::Equal(water_reflectance, command.arg_1_)) {
            water_reflectance = command.arg_1_;
        }

        glm::vec3 camera_position;
        camera_->GetPositionVector(camera_position.x, camera_position.y, camera_position.z);
        float time = glfwGetTime();

        shader_water_.Use();
        shader_water_.SetUniformMat4(shader_water_.uni_Model_, model);
        shader_water_.SetUniformVec3(shader_water_.uni_camera_world_position_, camera_position);
        shader_water_.SetUniformBool(shader_water_.uni_constant_tessellation_, constant_tessellation_);
        shader_water_.SetUniformFloat(shader_water_.uni_time_, time);
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("object_material.ambient"), ambient);
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("object_material.diffuse"), diffuse);
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("object_material.specular"), specular);
        shader_water_.SetUniformFloat(shader_water_.GetUniformLocation("object_material.shininess"), shininess);

        size_t nr_waves = std::min(waves.size(), 4u);
        shader_water_.SetUniformUInt(shader_water_.GetUniformLocation("number_of_waves"), nr_waves);
        for (size_t i = 0; i < nr_waves; i++) {
            std::string shader_name = "waves[" + std::to_string(i) + "]";
            shader_water_.SetUniformVec3(shader_water_.GetUniformLocation(shader_name + ".direction"), glm::normalize(waves[i].direction_));
            shader_water_.SetUniformFloat(shader_water_.GetUniformLocation(shader_name + ".wavelength"), waves[i].wavelength_);
            shader_water_.SetUniformFloat(shader_water_.GetUniformLocation(shader_name + ".amplitude"), waves[i].amplitude_);
        }
        shader_water_.SetUniformFloat(shader_water_.uni_environment_reflectance_, water_reflectance);

        object.SetupAttributes(&shader_displacement_);

        diffuse_texture->ActivateTexture(0);
        specular_texture->ActivateTexture(1);
        bump_texture->ActivateTexture(2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->depth_texture_);
        skybox_->ActivateTexture(4);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.element_buffer_);
        glDrawElements(GL_PATCHES, object.total_indices_, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        return 0;
    }

    int OpenGLRenderer::DrawGBufferTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color) {
        if (!is_inited_) return -1;
    
        OpenGLTriangle t;
        t.Init(v1, v2, v3);
    
        if (!t.IsInited()) return -1;
    
        glDisable(GL_CULL_FACE);
    
        shader_gbuffer_.Use();
        /* Set the model uniform */
        shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_Model_, glm::mat4(1.0f));
        //shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.ambient"), mtl.ambient_);
        shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.diffuse"), color);
        shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.specular"), color);
        //shader_gbuffer_.SetUniformFloat(shader_gbuffer_.GetUniformLocation("object_material.shininess"), mtl.shininess_);
    
        glBindVertexArray(t.VAO_);
    
        t.SetupAttributes(&shader_gbuffer_);
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_empty_->GetID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_empty_->GetID());
    
        t.Render();
    
        /* Unbind */
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    
        t.Destroy();
        glEnable(GL_CULL_FACE);
    
        return 0;
    }

    int OpenGLRenderer::DrawShadowMap(OpenGLObject & object, glm::mat4 model) {
    
        if (!is_inited_) return -1;
        if (!object.IsInited()) return -1;
    
        shader_shadow_map_.Use();
        /* Set the model uniform */
        shader_shadow_map_.SetUniformMat4(shader_shadow_map_.uni_Model_, model);
        
        glBindVertexArray(object.VAO_);
    
        object.SetupAttributes(&shader_shadow_map_);
        object.Render();
    
        /* Unbind */
        glBindVertexArray(0);
    
        return 0;
    }
    
    int OpenGLRenderer::DrawSSAO() {
    
        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_SSAO_RADIUS && !math::Equal(ssao_radius_used_, command.arg_1_)) {
            ssao_radius_used_ = command.arg_1_;
        }
        else if (command.type_ == COMMAND_SSAO_SAMPLES && command.arg_1_ <= 128 && ssao_samples_used_ != static_cast<int>(command.arg_1_)) {
            ssao_samples_used_ = static_cast<int>(command.arg_1_);
        }
        else if (command.type_ == COMMAND_SSAO_INTENSITY && !math::Equal(ssao_intensity_, command.arg_1_)) {
            ssao_intensity_ = command.arg_1_;
        }
        else if (command.type_ == COMMAND_SSAO_BIAS && !math::Equal(ssao_bias_, command.arg_1_)) {
            ssao_bias_ = command.arg_1_;
        }
    
        shader_ssao_.Use();
        shader_ssao_.SetUniformFloat(shader_ssao_.uni_radius_, ssao_radius_used_);
        shader_ssao_.SetUniformInt(shader_ssao_.uni_samples_size_, ssao_samples_used_);
        shader_ssao_.SetUniformFloat(shader_ssao_.uni_intensity_, ssao_intensity_);
        shader_ssao_.SetUniformFloat(shader_ssao_.uni_bias_, ssao_bias_);
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_position_texture_);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_normal_texture_);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noise_texture_);
    
        RenderQuad();
    
        return 0;
    }
    
    int OpenGLRenderer::DrawSeparableAO() {
    
        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_SSAO_RADIUS && !math::Equal(ssao_radius_used_, command.arg_1_)) {
            ssao_radius_used_ = command.arg_1_;
        }
        else if (command.type_ == COMMAND_SSAO_SAMPLES && command.arg_1_ <= 128 && separable_ao_samples_used_ != static_cast<int>(command.arg_1_)) {
            separable_ao_samples_used_ = static_cast<int>(command.arg_1_);
        }
        else if (command.type_ == COMMAND_SSAO_INTENSITY && !math::Equal(ssao_intensity_, command.arg_1_)) {
            ssao_intensity_ = command.arg_1_;
        }
        else if (command.type_ == COMMAND_SSAO_BIAS && !math::Equal(ssao_bias_, command.arg_1_)) {
            ssao_bias_ = command.arg_1_;
        }
    
        shader_separable_ao_.Use();
        shader_separable_ao_.SetUniformFloat(shader_separable_ao_.uni_radius_, ssao_radius_used_);
        shader_separable_ao_.SetUniformInt(shader_separable_ao_.uni_samples_size_, separable_ao_samples_used_);
        shader_separable_ao_.SetUniformFloat(shader_separable_ao_.uni_intensity_, ssao_intensity_);
        shader_separable_ao_.SetUniformFloat(shader_separable_ao_.uni_bias_, ssao_bias_);
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_position_texture_);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_normal_texture_);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noise_texture_);
    
        RenderQuad();
    
        return 0;
    }

    int OpenGLRenderer::DrawSkybox(OpenGLCubemap * skybox)
    {
        glBindVertexArray(skybox_cube_->VAO_);

        shader_skybox_.Use();
        skybox->ActivateTexture(0);

        glDepthFunc(GL_LEQUAL);
        skybox_cube_->Render();
        glDepthFunc(GL_LESS); 

        return 0;
    }
    
    int OpenGLRenderer::BlurTexture(GLuint texture_id) {
    
        shader_blur_.Use();
    
        ConsoleCommand command = ConsoleParser::GetInstance().GetLastCommand();
        if (command.type_ == COMMAND_SSAO_BLUR_SIZE && blur_kernel_size_ != static_cast<int>(command.arg_1_)) {
            blur_kernel_size_ = static_cast<int>(command.arg_1_);
            shader_blur_.SetUniformInt(shader_blur_.GetUniformLocation(shader_blur_kernel_size), blur_kernel_size_);
        }
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        RenderQuad();
    
        return 0;
    }
    
    int OpenGLRenderer::DrawFinalPass(GLuint ssao_texture) {
    
        shader_final_pass_.Use();
        shader_final_pass_.SetUniformBool(shader_final_pass_.uni_use_shadows_, use_shadows_);
        shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_lightspace_0_, shadow_map_->GetLightspaceMatrix(0));
        shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_lightspace_1_, shadow_map_->GetLightspaceMatrix(1));
        shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_lightspace_2_, shadow_map_->GetLightspaceMatrix(2));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_position_texture_);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_normal_texture_);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, g_buffer_->g_albedo_spec_texture_);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ssao_texture);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, shadow_map_->shadow_maps[0]);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, shadow_map_->shadow_maps[1]);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, shadow_map_->shadow_maps[2]);

        RenderQuad();
    
        return 0;
    }

    int OpenGLRenderer::DrawBoundingBox(OpenGLObject & object, glm::mat4 model, bool faces) {
    
        if (!object.IsInited()) return -1;
    
        glm::mat4 final_model = model * object.bbox_transform_;
    
        shader_vertices_color_.Use();
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Model_, final_model);
        shader_vertices_color_.SetUniformVec3(shader_vertices_color_.uni_fragment_color_, glm::vec3(1.0f, 0.0f, 0.0f));
    
        /* Bind bounding box vertex array */
        if (faces) glBindVertexArray(object.VAO_bbox_faces_);
        else glBindVertexArray(object.VAO_bbox_);
    
        /* Setup bounding box vertices atribute */
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_buffer_bbox_);
        /* Attribute number 0 is the object vertices */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader_vertices_color_.GetAttributeLocation(shader_vertex_position), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
        if (faces) object.RenderBoundingBoxFaces();
        else object.RenderBoundingBoxLines();
    
        /* Unbind */
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
        return 0;
    }
    
    int OpenGLRenderer::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
        
        shader_vertices_color_.Use();
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Model_, glm::mat4(1.0));
        shader_vertices_color_.SetUniformVec3(shader_vertices_color_.uni_fragment_color_, color);
    
        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    
        GLfloat line_vertices[] = {
          start.x, start.y, start.z, 
          end.x, end.y, end.z,
        };
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
    
        GLushort line_elements[] = {
          0, 1
        };
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_elements), line_elements, GL_STATIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(shader_vertices_color_.GetAttributeLocation(shader_vertex_position), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
        glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_SHORT, 0);
        // I commened this line when i was working on the particle springs
        //glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    
        return 0;
    }
    
    int OpenGLRenderer::SetPointLightsNumber(unsigned int number) {
    
        shader_final_pass_.Use();
        shader_final_pass_.SetUniformUInt(shader_final_pass_.GetUniformLocation("number_of_point_lights"), number);
    
        return 0;
    }
    
    int OpenGLRenderer::SetPointLight(std::string index, glm::vec3 position, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular,
        float attenuation_constant, float attenuation_linear, float attenuation_quadratic)
    {
        std::string shader_name = "point_light[" + index + "]";
    
        shader_final_pass_.Use();
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation(shader_name + ".position"), position);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation(shader_name + ".ambient"), color_ambient);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation(shader_name + ".diffuse"), color_diffuse);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation(shader_name + ".specular"), color_specular);
    
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation(shader_name + ".constant"), attenuation_constant);
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation(shader_name + ".linear"), attenuation_linear);
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation(shader_name + ".quadratic"), attenuation_quadratic);
    
        return 0;
    }

    int OpenGLRenderer::SetDirectionalLight(glm::vec3 direction, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular) {
    
        shader_final_pass_.Use();
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.direction"), direction);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.ambient"), color_ambient);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.diffuse"), color_diffuse);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.specular"), color_specular);
        
        shader_standard_.Use();
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("directional_light.direction"), direction);
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("directional_light.ambient"), color_ambient);
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("directional_light.diffuse"), color_diffuse);
        shader_standard_.SetUniformVec3(shader_standard_.GetUniformLocation("directional_light.specular"), color_specular);

        shader_water_.Use();
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("directional_light.direction"), direction);
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("directional_light.ambient"), color_ambient);
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("directional_light.diffuse"), color_diffuse);
        shader_water_.SetUniformVec3(shader_water_.GetUniformLocation("directional_light.specular"), color_specular);

        return 0;
    }
    
    int OpenGLRenderer::SetSpotLight(glm::vec3 position, glm::vec3 direction, float inner_radius, float outer_radius,
        glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular,
        float attenuation_constant, float attenuation_linear, float attenuation_quadratic)
    {
        shader_final_pass_.Use();
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("cast_light.position"), position);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("cast_light.direction"), direction);
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation("cast_light.inner_radius_cosine"), cos(inner_radius));
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation("cast_light.outer_radius_cosine"), cos(outer_radius));
    
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("cast_light.ambient"), color_ambient);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("cast_light.diffuse"), color_diffuse);
        shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("cast_light.specular"), color_specular);
    
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation("cast_light.constant"), attenuation_constant);
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation("cast_light.linear"), attenuation_linear);
        shader_final_pass_.SetUniformFloat(shader_final_pass_.GetUniformLocation("cast_light.quadratic"), attenuation_quadratic);
    
        return 0;
    }
    
    int OpenGLRenderer::Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    
        text_renderer_->Draw2DText(text, x, y, scale, color);
        return 0;
    }
    
    int OpenGLRenderer::DrawTexture(GLuint texture_id, bool red_component) {
        shader_quad_.Use();
        shader_quad_.SetUniformBool(shader_quad_.GetUniformLocation("red_component"), red_component);
    
        glDisable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    
        RenderQuad();
        
        glEnable(GL_DEPTH_TEST);
        return 0;
    }

    int OpenGLRenderer::DrawNormals(OpenGLObject & object, glm::mat4 model, glm::vec3 color)
    {
        if (!is_inited_) return -1;
        if (!object.IsInited()) return -1;

        shader_draw_normals_.Use();
        shader_draw_normals_.SetUniformMat4(shader_draw_normals_.uni_Model_, model);
        shader_draw_normals_.SetUniformVec3(shader_draw_normals_.uni_color_, color);

        glBindVertexArray(object.VAO_);

        object.SetupAttributes(&shader_draw_normals_);
        object.Render();

        /* Unbind */
        glBindVertexArray(0);

        return 0;
    }

    int OpenGLRenderer::DrawColor(OpenGLObject & object, glm::mat4 & model, glm::vec3 color, float alpha)
    {
        glBindVertexArray(object.VAO_);

        shader_vertices_color_.Use();
        shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Model_, model);
        shader_vertices_color_.SetUniformVec3(shader_vertices_color_.uni_fragment_color_, color);
        shader_vertices_color_.SetUniformFloat(shader_vertices_color_.uni_fragment_alpha_, alpha);
        object.SetupAttributes(&shader_vertices_color_);

        object.Render();

        glBindVertexArray(0);
        return 0;
    }
    
    void OpenGLRenderer::RenderQuad() {
        
        glBindVertexArray(VAO_Quad_);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

}
}
}