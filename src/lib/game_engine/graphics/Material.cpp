#include "Material.hpp"

#include "AssetManager.hpp"

namespace game_engine { namespace graphics {
    
    /*
        0 = GBuffer pass
        1 = Forward pass
        2 = Overlay
    */

    MaterialDeferredStandard::MaterialDeferredStandard(game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, std::string texture_diffuse, std::string texture_specular)
    {
        diffuse_ = diffuse;
        specular_ = specular;

        AssetManager& instance = AssetManager::GetInstance();
        texture_diffuse_ = instance.GetTexture(texture_diffuse, GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP);
        texture_specular_ = instance.GetTexture(texture_specular, GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP);

        rendering_queue_ = 0;
    }

    void MaterialDeferredStandard::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawGBuffer(object, model, diffuse_.ToGlm(), specular_.ToGlm(), texture_diffuse_, texture_specular_);
    }

    void MaterialDeferredStandard::RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawShadowMap(object, model);
    }

    MaterialDeferredDisplacement::MaterialDeferredDisplacement(game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, std::string texture_displacement)
    {
        diffuse_ = diffuse;
        specular_ = specular;

        AssetManager& instance = AssetManager::GetInstance();
        texture_displacement_ = instance.GetTexture(texture_displacement, GAME_ENGINE_TEXTURE_TYPE_DISPLACEMENT_MAP);

        rendering_queue_ = 0;
    }

    void MaterialDeferredDisplacement::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawDisplacement(object, model, texture_displacement_, displacement_intensity_);
    }

    void MaterialDeferredDisplacement::RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        /* TODO Tesselation shader shadow map not implemented */
        return;
    }

    MaterialForwardDrawNormals::MaterialForwardDrawNormals(game_engine::math::Vector3D color)
    {
        color_ = color;

        rendering_queue_ = 1;
    }

    void MaterialForwardDrawNormals::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawNormals(object, model, color_.ToGlm());
    }

    void MaterialForwardDrawNormals::RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        /* empty */
    }

    MaterialForwardColor::MaterialForwardColor(game_engine::math::Vector3D color)
    {
        color_ = color;

        rendering_queue_ = 1;
    }

    void MaterialForwardColor::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        renderer->DrawColor(object, model, color_.ToGlm(), alpha_);
        glDisable(GL_BLEND);
    }

    void MaterialForwardColor::RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        /* empty */
    }


    MaterialForwardDisplacementDrawNormals::MaterialForwardDisplacementDrawNormals(game_engine::math::Vector3D normal_color, std::string texture_displacement)
    {
        normal_color_ = normal_color;

        AssetManager& instance = AssetManager::GetInstance();
        texture_displacement_ = instance.GetTexture(texture_displacement, GAME_ENGINE_TEXTURE_TYPE_DISPLACEMENT_MAP);

        rendering_queue_ = 1;
    }

    void MaterialForwardDisplacementDrawNormals::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawDisplacementNormals(object, model, texture_displacement_, displacement_intensity_, normal_color_.ToGlm());
    }

    void MaterialForwardDisplacementDrawNormals::RenderShadow(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
    }

}

}