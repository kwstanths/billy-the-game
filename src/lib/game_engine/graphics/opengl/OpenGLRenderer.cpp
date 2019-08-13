#include "OpenGLRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "game_engine/ConsoleParser.hpp"
#include "game_engine/math/RNG.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/graphics/GraphicsTypes.hpp"

#include "debug_tools/Console.hpp"

namespace math = game_engine::math;


namespace game_engine {
    namespace graphics {
        namespace opengl {

            OpenGLRenderer::OpenGLRenderer() {

                is_inited_ = false;
                text_renderer_ = new OpenGLText();
                g_buffer_ = new OpenGLGBuffer();
                frame_buffer_one_ = new OpenGLFrameBufferTexture();
                frame_buffer_two_ = new OpenGLFrameBufferTexture();

            }

            int OpenGLRenderer::Init(OpenGLContext * context) {
                context_ = context;

                /* Initialize font */
                std::string font_location = context->GetFontLocation();
                if (text_renderer_->Init(font_location, context) != 0)
                    debug_tools::ConsoleInfoL(debug_tools::CRITICAL, "Font not found", "name", font_location);

                /* Get shader variables */
                shader_vertices_color_ = context_->shader_vertices_color_;

                {
                    shader_main_ = context_->shader_main_;
                    shader_main_.Use();
                    shader_main_.SetUniformInt(shader_main_.GetUniformLocation("object_material.texture_diffuse"), 0);
                    shader_main_.SetUniformInt(shader_main_.GetUniformLocation("object_material.texture_specular"), 1);
                }

                {
                    shader_model_texture_ = context_->shader_model_texture_;
                    shader_model_texture_.Use();
                    shader_model_texture_.SetUniformInt(shader_model_texture_.uni_texture_, 0);
                    shader_model_texture_.SetUniformInt(shader_model_texture_.uni_shadow_map_, 1);
                }

                {
                    /* Used to draw textures */
                    shader_quad_ = context_->shader_quad_;
                    shader_quad_.Use();
                    shader_quad_.SetUniformInt(shader_quad_.GetUniformLocation(shader_sampler_texture), 0);
                }

                {
                    /* The G buffer shader */
                    shader_gbuffer_ = context_->shader_gbuffer_ssao_;
                    shader_gbuffer_.Use();
                    shader_gbuffer_.SetUniformInt(shader_gbuffer_.GetUniformLocation("object_material.texture_diffuse"), 0);
                    shader_gbuffer_.SetUniformInt(shader_gbuffer_.GetUniformLocation("object_material.texture_specular"), 1);
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
                }

                /* Init GBuffer */
                g_buffer_->Init(context_);
                /* Init other frame buffers */
                frame_buffer_one_->Init(context_, GL_RED);
                frame_buffer_two_->Init(context_, GL_RED);


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

            void OpenGLRenderer::SetView(OpenGLCamera * camera) {

                camera_ = camera;

                camera->CalculateView();

                shader_main_.Use();
                shader_main_.SetUniformMat4(shader_main_.uni_View_, camera->view_matrix_);
                shader_main_.SetUniformMat4(shader_main_.uni_Projection_, camera->projection_matrix_);
                shader_main_.SetUniformVec3(shader_main_.uni_camera_position_worldspace_, camera->config_.position_);

                shader_model_texture_.Use();
                shader_model_texture_.SetUniformMat4(shader_model_texture_.uni_View_, camera->view_matrix_);
                shader_model_texture_.SetUniformMat4(shader_model_texture_.uni_Projection_, camera->projection_matrix_);

                shader_vertices_color_.Use();
                shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_View_, camera->view_matrix_);
                shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Projection_, camera->projection_matrix_);

                shader_gbuffer_.Use();
                shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_View_, camera->view_matrix_);
                shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_Projection_, camera->projection_matrix_);

                shader_ssao_.Use();
                shader_ssao_.SetUniformMat4(shader_ssao_.uni_matrix_projection_, camera->projection_matrix_);

                shader_separable_ao_.Use();
                shader_separable_ao_.SetUniformMat4(shader_separable_ao_.uni_matrix_projection_, camera->projection_matrix_);

                shader_final_pass_.Use();
                shader_final_pass_.SetUniformMat4(shader_final_pass_.uni_matrix_view_, camera->view_matrix_);
            }

            int OpenGLRenderer::Draw(OpenGLObject & object, std::vector<OpenGLTexture *> & textures, glm::mat4 model, Material_t mtl) {

                /* TODO Check input arguments */

                shader_main_.Use();
                /* Set object material */
                shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.ambient"), mtl.ambient_);
                shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.diffuse"), mtl.diffuse_);
                shader_main_.SetUniformVec3(shader_main_.GetUniformLocation("object_material.specular"), mtl.specular_);
                shader_main_.SetUniformFloat(shader_main_.GetUniformLocation("object_material.shininess"), mtl.shininess_);
                /* Set the model uniform */
                shader_main_.SetUniformMat4(shader_main_.uni_Model_, model);

                glBindVertexArray(object.VAO_);

                object.SetupAttributes(&shader_main_);

                for (size_t i = 0; i < textures.size(); i++) {
                    textures[i]->ActivateTexture(i);
                }

                /* Draw with index buffer */
                object.Render();

                /* Unbind everything */
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindVertexArray(0);
                return 0;
            }

            int OpenGLRenderer::Draw(OpenGLObject & object, OpenGLTexture * texture, glm::mat4 model) {

                if (!is_inited_) return -1;
                if (!object.IsInited()) return -1;
                if (!texture->IsInited()) return -1;

                shader_model_texture_.Use();
                /* Set the model uniform */
                shader_model_texture_.SetUniformMat4(shader_model_texture_.uni_Model_, model);

                glBindVertexArray(object.VAO_);

                object.SetupAttributes(&shader_model_texture_);
                texture->ActivateTexture(0);
                object.Render();

                /* Unbind */
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindVertexArray(0);
                shader_main_.Use();

                return 0;
            }

            int OpenGLRenderer::DrawGBuffer(OpenGLObject & object, std::vector<OpenGLTexture *> & textures, glm::mat4 model, Material_t mtl) {

                if (!is_inited_) return -1;
                if (!object.IsInited()) return -1;

                shader_gbuffer_.Use();
                /* Set the model uniform */
                shader_gbuffer_.SetUniformMat4(shader_gbuffer_.uni_Model_, model);
                //shader_gbuffer_.SetUniformVec3(shader_gbuffer_ssao_.GetUniformLocation("object_material.ambient"), mtl.ambient_);
                shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.diffuse"), mtl.diffuse_);
                shader_gbuffer_.SetUniformVec3(shader_gbuffer_.GetUniformLocation("object_material.specular"), mtl.specular_);
                //shader_gbuffer_.SetUniformFloat(shader_gbuffer_.GetUniformLocation("object_material.shininess"), mtl.shininess_);

                glBindVertexArray(object.VAO_);

                object.SetupAttributes(&shader_gbuffer_);

                for (size_t i = 0; i < textures.size(); i++) {
                    textures[i]->ActivateTexture(i);
                }
                object.Render();

                /* Unbind */
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindVertexArray(0);
                shader_main_.Use();

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

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, g_buffer_->g_position_texture_);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, g_buffer_->g_normal_texture_);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, g_buffer_->g_albedo_spec_texture_);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, ssao_texture);

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
                shader_main_.Use();

                return 0;
            }

            int OpenGLRenderer::DrawLine(glm::vec3 start, glm::vec3 end) {
                
                shader_vertices_color_.Use();
                shader_vertices_color_.SetUniformMat4(shader_vertices_color_.uni_Model_, glm::mat4(1.0));
                shader_vertices_color_.SetUniformVec3(shader_vertices_color_.uni_fragment_color_, glm::vec3(1.0f, 1.0f, 1.0f));

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
                glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                glDeleteBuffers(1, &vbo);
                glDeleteBuffers(1, &ebo);
                glDeleteVertexArrays(1, &vao);

                return 0;
            }

            int OpenGLRenderer::SetPointLightsNumber(unsigned int number) {

                shader_final_pass_.Use();
                shader_final_pass_.SetUniformUInt(shader_final_pass_.GetUniformLocation("number_of_point_lights"), number);

                //shader_main_.Use();
                //shader_main_.SetUniformUInt(shader_main_.GetUniformLocation("number_of_point_lights"), number);

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

                //shader_main_.Use();
                //shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".position"), position);
                //shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".ambient"), color_ambient);
                //shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".diffuse"), color_diffuse);
                //shader_main_.SetUniformVec3(shader_main_.GetUniformLocation(shader_name + ".specular"), color_specular);
                //
                //shader_main_.SetUniformFloat(shader_main_.GetUniformLocation(shader_name + ".constant"), attenuation_constant);
                //shader_main_.SetUniformFloat(shader_main_.GetUniformLocation(shader_name + ".linear"), attenuation_linear);
                //shader_main_.SetUniformFloat(shader_main_.GetUniformLocation(shader_name + ".quadratic"), attenuation_quadratic);

                return 0;
            }

            int OpenGLRenderer::SetDirectionalLight(glm::vec3 direction, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular) {

                shader_final_pass_.Use();
                shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.direction"), direction);
                shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.ambient"), color_ambient);
                shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.diffuse"), color_diffuse);
                shader_final_pass_.SetUniformVec3(shader_final_pass_.GetUniformLocation("directional_light.specular"), color_specular);

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

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_id);

                RenderQuad();

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