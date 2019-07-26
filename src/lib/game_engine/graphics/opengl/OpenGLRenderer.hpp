#ifndef __OpenGLRenderer_hpp__
#define __OpenGLRenderer_hpp__

#include <vector>

#include "glm/glm.hpp"

#include "game_engine/graphics/GraphicsTypes.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLShaders.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLObject.hpp"
#include "OpenGLText.hpp"
#include "OpenGLCamera.hpp"
#include "OpenGLGBuffer.hpp"
#include "OpenGLFrameBufferTexture.hpp"

namespace game_engine {
    namespace graphics {
        namespace opengl {

            /* Maximum number of point lights allowed */
#define GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS 32

            class OpenGLRenderer {
            public:
                /**
                    Does nothing in particular
                */
                OpenGLRenderer();

                /**
                    Initializes the renderer
                    @parma context An OpenGL context, mainly for the shader variables
                    @return 0=OK
                */
                int Init(OpenGLContext * context);

                /**
                    Destroys allocated data
                    @return 0=OK
                */
                int Destroy();

                /**
                    Return whether the object is initialised
                */
                bool IsInited();

                void EnableColorWriting(bool enable);

                void EnableDepthWriting(bool enable);

                /**
                    Set the camera related matrices, for all the shaders that need them
                */
                void SetView(OpenGLCamera * camera);

                /**
                    Draw an object on the scene with lightning
                    @param object The object to be drawn
                    @param diffuse_texture The texture to draw
                    @param specular_texture The specular texture to be used
                    @param model The object's model
                    @param mtl The material
                    @return 0=OK, -1=Something's not initialised;
                */
                int Draw(OpenGLObject & object, std::vector<OpenGLTexture *> & textures, glm::mat4 model, Material_t mtl);

                /**
                    Draw an object on the scene without lightning
                    @param object The object to be drawn
                    @param textuer The texture to be used
                    @param model The object's model
                */
                int Draw(OpenGLObject & object, OpenGLTexture * texture, glm::mat4 model);

                /**
                    Draws the object using a GBuffer
                */
                int DrawGBuffer(OpenGLObject & object, std::vector<OpenGLTexture *> & textures, glm::mat4 model, Material_t mtl);

                /**

                */
                int DrawSSAO();

                /**
                    If vertical is set, the output texture of framebuffer two will be used as input for the horizontal pass
                    So bind something else
                */
                int DrawSeparableAO();

                /**

                */
                int BlurTexture(GLuint texture_id);

                /**

                */
                int DrawFinalPass(GLuint ssao_texture);

                /**

                */
                int DrawBoundingBox(OpenGLObject & object, glm::mat4 model, bool faces = true);

                /**
                
                */
                int DrawLine(glm::vec3 start, glm::vec3 end);

                /**

                */
                int SetPointLightsNumber(unsigned int number);

                /**

                */
                int SetPointLight(std::string index, glm::vec3 position, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular,
                    float attenuation_constant, float attenuation_linear, float attenuation_quadratic);

                /**

                */
                int SetDirectionalLight(glm::vec3 direction, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular);

                /**
                    @param radius The spotlight angle between the light direction and the edge of the cone in radians
                */
                int SetSpotLight(glm::vec3 position, glm::vec3 direction, float inner_radius, float outer_radius,
                    glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular,
                    float attenuation_constant, float attenuation_linear, float attenuation_quadratic);

                /**
                    Draw a 2d text on a certain position on the screen, not in the world
                    @param text The text to draw
                    @param x The horizontal position in screen coordinates
                    @param y The vertical position in screen coordintates
                    @param scale The size of the text
                    @param color The color of the text in RGB format
                    @return 0=OK, -1=Font was not initialised
                */
                int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

                /**
                    Draws a texture
                    @param texture_id The id of the texture
                    @param red_component If true, the red component will be used to output a gray scale color
                */
                int DrawTexture(GLuint texture_id, bool red_component = false);


                OpenGLGBuffer * g_buffer_;
                OpenGLFrameBufferTexture * frame_buffer_one_;
                OpenGLFrameBufferTexture * frame_buffer_two_;

            private:
                bool is_inited_;

                OpenGLContext * context_;
                OpenGLText * text_renderer_;
                OpenGLCamera * camera_;

                size_t number_of_samples_;
                std::vector<glm::vec3> random_samples_kernel_;

                size_t noise_size_;
                std::vector<glm::vec3> noise_;
                GLuint noise_texture_;

                float ssao_radius_used_;
                int ssao_samples_used_;
                int separable_ao_samples_used_;
                float ssao_intensity_;
                float ssao_bias_;
                int blur_kernel_size_;

                /* Main shaders */
                OpenGLShaderMain shader_main_;
                /* Model texture only shaders */
                OpenGLShaderModelTexture shader_model_texture_;
                /* Verices color only shaders */
                OpenGLShaderVerticesColor shader_vertices_color_;
                /* texture drawing shader */
                OpenGLShader shader_quad_;
                /* gbuffer shader used */
                OpenGLShaderGBuffer shader_gbuffer_;
                /* SSAO shader */
                OpenGLShaderSSAO shader_ssao_;
                /* Separable AO shader */
                OpenGLShaderSeparableAO shader_separable_ao_;
                /* bluring shader */
                OpenGLShader shader_blur_;
                /* Final pass rendering */
                OpenGLShaderFinalPass shader_final_pass_;

                /* Framebuffer object */
                GLuint VAO_Quad_;
                GLuint VBO_Quad_;

                /**
                    Sends a quad geometry to the currently bound shader
                    Location 0 is position, Location 1 is uv coordinates
                */
                void RenderQuad();
            };

        }
    }
}

#endif