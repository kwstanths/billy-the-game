#ifndef __Material_hpp__
#define __Material_hpp__

#include <string>

#include "opengl/OpenGLRenderer.hpp"
#include "opengl/OpenGLObject.hpp"

#include "game_engine/math/Vector.hpp"

namespace game_engine { namespace graphics {

    /* Base material */
    class Material {
    public:
        
        virtual void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) = 0;

        virtual void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) = 0;

        size_t rendering_queue_;
    };

    /**
        Material used for standard deferred rendering pass
    */
    class MaterialDeferredStandard : public Material {
    public:
        MaterialDeferredStandard(game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, std::string texture_diffuse, std::string texture_specular);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        game_engine::math::Vector3D diffuse_;
        game_engine::math::Vector3D specular_;
        opengl::OpenGLTexture * texture_diffuse_;
        opengl::OpenGLTexture * texture_specular_;
    };

    class MaterialDeferredDisplacement : public Material {
    public:
        MaterialDeferredDisplacement(game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, std::string texture_displacement, std::string texture_normal);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        game_engine::math::Vector3D diffuse_;
        game_engine::math::Vector3D specular_;
        opengl::OpenGLTexture * texture_displacement_;
        opengl::OpenGLTexture * texture_normal_;
        Real_t displacement_intensity_ = 1;
    };

}
}




#endif
