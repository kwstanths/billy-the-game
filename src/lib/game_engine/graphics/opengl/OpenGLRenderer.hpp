#ifndef __OpenGLRenderer_hpp__
#define __OpenGLRenderer_hpp__

#include "glm/glm.hpp"

#include "game_engine/graphics/Material.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLShaders.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLObject.hpp"
#include "OpenGLFont.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

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
            Draw an object on the scene
            @param object The object to be drawn
            @param texture The texture to draw
            @param model The object's model
            @return 0=OK, -1=Something's not initialised;
        */
        int Draw(OpenGLObject * object, OpenGLTexture * texture, glm::mat4 model, Material_t mtl);
        
        /**
        
        */
        int SetLight(glm::vec3 position, glm::vec3 color_ambient, glm::vec3 color_diffuse, glm::vec3 color_specular);

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
        GLuint VAO_;
        OpenGLShaderMain shader_main_;
        
        /* Text shaders */
        GLuint VAO2DText_, VBO2DText_;
        glm::mat4 text_projection_matrix_;
        OpenGLShaderText shader_text_;
    };

}
}
}

#endif