#ifndef __Material_hpp__
#define __Material_hpp__

#include <string>

#include "opengl/OpenGLRenderer.hpp"
#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLCubemap.hpp"

#include "GraphicsTypes.hpp"

#include "game_engine/math/Vector.hpp"

namespace game_engine { namespace graphics {

    /* Base material */
    class Material {
    public:
        
        virtual void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) = 0;

        virtual void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) = 0;

        size_t rendering_queue_;
        bool instancing_;
    };


    /**
        Material used for standard drawing, deferred pass
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

    /** 
        Material used for standard drawing, forward pass
    */
    class MaterialForwardStandard : public Material {
    public:
        MaterialForwardStandard(game_engine::math::Vector3D ambient, game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, Real_t shininess, std::string texture_diffuse, std::string texture_specular);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        game_engine::math::Vector3D ambient_;
        game_engine::math::Vector3D diffuse_;
        game_engine::math::Vector3D specular_;
        Real_t shininess_;
        opengl::OpenGLTexture * texture_diffuse_;
        opengl::OpenGLTexture * texture_specular_;
    };

    /**
        Material used for displcament map rendering, deferred pass
    */
    class MaterialDeferredDisplacement : public Material {
    public:
        MaterialDeferredDisplacement(Real_t specular_intensity, std::string texture_displacement, std::string texture_diffuse);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        Real_t specular_intensity_;
        opengl::OpenGLTexture * texture_displacement_;
        opengl::OpenGLTexture * texture_diffuse_;
        Real_t displacement_intensity_ = 1;
    };

    /**
        Material used for rendering the normals of a displacement map, deferred pass
    */
    class MaterialForwardDisplacementDrawNormals : public Material {
    public:
        MaterialForwardDisplacementDrawNormals (game_engine::math::Vector3D normal_color, std::string texture_displacement);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        game_engine::math::Vector3D normal_color_;
        opengl::OpenGLTexture * texture_displacement_;
        Real_t displacement_intensity_ = 1;
    };

    /**
        Material used to draw the normals of an object, forward pass
    */
    class MaterialForwardDrawNormals : public Material {
    public:
        MaterialForwardDrawNormals(game_engine::math::Vector3D color);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        game_engine::math::Vector3D color_;
    };

    /**
        Material used to draw a mesh with a single color, forward pass
    */
    class MaterialForwardColor : public Material {
    public:
        MaterialForwardColor(game_engine::math::Vector3D color);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        Real_t alpha_ = 1;
        game_engine::math::Vector3D color_;
    };

    /* Material used to draw water, forward pass */
    class MaterialForwardWater : public Material {
    public:

        MaterialForwardWater(game_engine::math::Vector3D ambient, game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, Real_t shininess, std::string texture_diffuse, std::string texture_specular, std::string texture_bump);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        game_engine::math::Vector3D ambient_;
        game_engine::math::Vector3D diffuse_;
        game_engine::math::Vector3D specular_;
        Real_t shininess_;
        opengl::OpenGLTexture * texture_diffuse_;
        opengl::OpenGLTexture * texture_specular_;
        opengl::OpenGLTexture * texture_bump_;
        std::vector<Wave_t> waves_;
    };

    /* Material used to draw a skybox, rendered last before overlay */
    class MaterialSkybox : public Material {
    public:
        MaterialSkybox(std::vector<std::string> faces);

        void Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;
        void RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model) override;

        opengl::OpenGLCubemap * texture_cubemap_;
    };

}
}




#endif
