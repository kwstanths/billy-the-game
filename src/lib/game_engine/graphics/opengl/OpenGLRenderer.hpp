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
#include "OpenGLCShadowMaps.hpp"
#include "OpenGLCubemap.hpp"

namespace game_engine { namespace graphics { namespace opengl {

    /* Maximum number of point lights allowed */
#define GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS 36

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
            Enable wireframe rendering mode
            @param enable True = enable, false = disable
        */
        void DrawWireframe(bool enable);
    
        /**
            Sets the necessary matrices for shadow mapping
        */
        void SetShadowMap(glm::mat4& matrix_lightspace);
    
        /**
            Set the camera related matrices, for all the shaders that need them
        */
        void SetView(OpenGLCamera * camera);
    
        /**
            Draws an object using the GBuffer
            @param object The mesh declared as an OpenGL vbo
            @param models_buffer The opengl buffer holding  the model matrix of the mesh
            @param amount The number of instances to draw, the models buffer should hold a model matrix for all of the instances
            @param diffuse The diffuse color
            @param specular The specular color
            @param diffuse_texture The diffuse texure
            @param specular_texture The specular texture
            @return 0 = OK, -1 = Something is not initialized
        */
        int DrawGBufferStandard(OpenGLObject & object, GLuint models_buffer, size_t amount, glm::vec3 diffuse, glm::vec3 specular, OpenGLTexture * diffuse_texture, OpenGLTexture * specular_texture);
        
        /**
            Draw a mesh with displacement using the GBuffer
        */
        int DrawGBufferDisplacement(OpenGLObject & object, glm::mat4 & model, float specular_intensity, OpenGLTexture * displacement_texture, float displacement_mult, OpenGLTexture * diffuse_texture);

        /**
            Draws an objects normals, forward rendering 
        */
        int DrawDisplacementNormals(OpenGLObject & object, glm::mat4 & model, OpenGLTexture * displacement_texture, float displacement_mult, glm::vec3 color);
    
        /**
            Draws an object using the standard shader, forward rendering, alpha value currently not processed, hardcoded as 1 in the shader 
        */
        int DrawStandard(OpenGLObject & object, GLuint models_buffer, size_t amount, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, OpenGLTexture * diffuse_texture, OpenGLTexture * specular_texture);

        /**
            Draws an object using the water shader, forward rendering
        */
        int DrawWater(OpenGLObject & object, glm::mat4 model, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, OpenGLTexture * diffuse_texture, OpenGLTexture * specular_texture, OpenGLTexture * bump_texture, std::vector<Wave_t>& waves);

        /**
            Draws the object with a single color, forward rendering
        */
        int DrawColor(OpenGLObject & object, glm::mat4 & model, glm::vec3 color, float alpha);

        /**
            
        */
        int DrawGBufferTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color = { 1,1,1 });
    
        /**
            Draws the objecet on the shadow map
        */
        int DrawShadowMap(OpenGLObject & object, GLuint models_buffer, size_t amount);
    
        /**
            Runs the SSAO algorithm
        */
        int DrawSSAO();
    
        /**
            Runs the separable AO algorithm
        */
        int DrawSeparableAO();

        /**
            Render a skybox
        */
        int DrawSkybox(OpenGLCubemap * skybox);
    
        /**
            Applys a bluring algorithm on a texture
        */
        int BlurTexture(GLuint texture_id);
    
        /**
            Runs a the final pass shader
        */
        int DrawFinalPass(GLuint ssao_texture);
    
        /**
            Draws the bounding box of an object
        */
        int DrawBoundingBox(OpenGLObject & object, glm::mat4 model, bool faces = true);
    
        /**
            Draws a line
        */
        int DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(1, 1, 1));
    
        /**
            Set the number of point lights
        */
        int SetPointLightsNumber(unsigned int number);
    
        /**
            Set a point light
        */
        int SetPointLight(std::string index, glm::vec3 position, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular,
            float attenuation_constant, float attenuation_linear, float attenuation_quadratic);
    
        /**
            Set a directional light
        */
        int SetDirectionalLight(glm::vec3 direction, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular);
    
        /**
            Set a spotlight
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
    
        /**
            Draw the normals of an object, forward rendering
            @param object The object
            @param model The model matrix of the object
        */
        int DrawNormals(OpenGLObject & object, glm::mat4 model, glm::vec3 color);
    
        OpenGLGBuffer * g_buffer_;
        OpenGLFrameBufferTexture * frame_buffer_one_;
        OpenGLFrameBufferTexture * frame_buffer_two_;
        OpenGLCShadowMaps * shadow_maps_;
        bool use_shadows_;
        OpenGLCubemap * skybox_ = nullptr;
    
    private:
        bool is_inited_;
    
        OpenGLContext * context_ = nullptr;
        OpenGLText * text_renderer_ = nullptr;
        OpenGLCamera * camera_ = nullptr;
        OpenGLSkyboxCube * skybox_cube_ = nullptr;

        /* AO parameters */
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
        bool constant_tessellation_ = false;
        float water_reflectance = 0.6f;
        bool show_shadow_cascades_ = false;
    
        /* Verices color only shaders */
        OpenGLShaderVerticesColor shader_vertices_color_;
        /* texture drawing shader */
        OpenGLShaderQuad shader_quad_;
        /* gbuffer shader used */
        OpenGLShaderGBuffer shader_gbuffer_;
        /* Standard forward shader */
        OpenGLShaderStandard shader_standard_;
        /* SSAO shader */
        OpenGLShaderSSAO shader_ssao_;
        /* Separable AO shader */
        OpenGLShaderSeparableAO shader_separable_ao_;
        /* Shadow map shader */
        OpenGLShaderShadowMap shader_shadow_map_;
        /* bluring shader */
        OpenGLShader shader_blur_;
        /* Final pass rendering */
        OpenGLShaderFinalPass shader_final_pass_;
        /* Shader used to draw the normals a model */
        OpenGLShaderDrawNormals shader_draw_normals_;
        /* Shader used to draw displacement textures */
        OpenGLShaderDisplacement shader_displacement_;
        /* Shader used to draw the normals of displaced mesh */
        OpenGLShaderDisplacementDrawNormals shader_displacement_draw_normals_;
        /* Shader used to draw water */
        OpenGLShaderWater shader_water_;
        /* Shader used to draw the skybox */
        OpenGLShaderSkybox shader_skybox_;

        /* VAO and VBO for quad rendering */
        GLuint VAO_Quad_;
        GLuint VBO_Quad_;
    
        /* Textures */
        OpenGLTexture * texture_empty_ = nullptr;
        
        /**
            Sends a quad geometry to the currently bound shader
            Location 0 is position, Location 1 is uv coordinates
        */
        void RenderQuad();

        /**
            Set the model matrix attribute for a shader, at a certain position
            @param position The shader attribute position
            @param buffer The ARRAY_BUFFER storing the model matrix data
        */
        void SetModelMatrixAttribute(GLuint position, GLuint buffer);
    };

}
}
}

#endif