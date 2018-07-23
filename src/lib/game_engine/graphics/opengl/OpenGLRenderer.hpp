#ifndef __OpenGLRenderer_hpp__
#define __OpenGLRenderer_hpp__

#include "glm/glm.hpp"

#include "game_engine/graphics/GraphicsTypes.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLShaders.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLObject.hpp"
#include "OpenGLFont.hpp"
#include "OpenGLCamera.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

/* Maximum number of point lights allowed */
#define GAME_ENGINE_GL_RENDERER_MAX_POINT_LIGHTS 16

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

        /**
        
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
        int Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model);

        /**
            
        */
        int DrawLineXY(float start_x, float start_y, float stop_x, float stop_y, float z_height, float size, glm::vec3 color);

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

    private:
        bool is_inited_;
        
        OpenGLContext * context_;
        OpenGLFont * font_;

        /* Main shaders */
        OpenGLShaderMain shader_main_;
        /* Model texture only shaders */
        OpenGLShaderModelTexture shader_model_texture_;
        /* Verices coloe only shaders */
        OpenGLShaderVerticesColor shader_vertices_color_;
        
        /* Text shaders */
        GLuint VAO2DText_, VBO2DText_;
        glm::mat4 text_projection_matrix_;
        OpenGLShaderText shader_text_;
    };

}
}
}

#endif