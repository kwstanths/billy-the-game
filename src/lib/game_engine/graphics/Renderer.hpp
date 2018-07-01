#ifndef __Renderer_hpp__
#define __Renderer_hpp__

#include "game_engine/physics/Types.hpp"
#include "game_engine/graphics/opengl/OpenGLContext.hpp"
#include "game_engine/graphics/opengl/OpenGLRenderer.hpp"
#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/opengl/OpenGLCamera.hpp"

#include "Material.hpp"
#include "GraphicsObject.hpp"

namespace game_engine {
namespace graphics {

    class Renderer {
    public:
        Renderer();

        int Init(opengl::OpenGLContext * context);

        bool IsInited();

        void SetView(opengl::OpenGLCamera * camera);

        void StartFrame();

        /**
            Draws an object with full lightning
            @param rendering_object The object to draw
            @return 0 = OK, -1 = rendering_object is not properly initialised
        */
        int Draw(GraphicsObject * rendering_object);

        /**
            Draws an object as is
            @param object The OpenGLObject
            @param texture The OpenGLTexture
            @param model The model of the object
            @return 0 = OK, -1 = something is not properly initialised
        */
        int Draw(opengl::OpenGLObject * object, opengl::OpenGLTexture * texture, glm::mat4 model);

        int AddLight(glm::vec3 position, LightProperties_t light_properties, Attenuation_t attenuation);

        int AddDirectionalLight(glm::vec3 direction, LightProperties_t light_properties);

        /**
            @param angles in degrees
        */
        int AddSpotLight(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle,
            LightProperties_t light_properties, Attenuation_t attenuation);

        int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        bool is_inited_;
        opengl::OpenGLRenderer * renderer_;

    };

}
}

#endif