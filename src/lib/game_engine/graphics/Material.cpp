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

        size_t rendering_queue_ = 0;
    }

    void MaterialDeferredStandard::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawGBuffer(object, model, diffuse_.ToGlm(), specular_.ToGlm(), texture_diffuse_, texture_specular_);
    }

    MaterialDeferredDisplacement::MaterialDeferredDisplacement(game_engine::math::Vector3D diffuse, game_engine::math::Vector3D specular, std::string texture_displacement, std::string texture_normal)
    {
        diffuse_ = diffuse;
        specular_ = specular;

        AssetManager& instance = AssetManager::GetInstance();
        texture_displacement_ = instance.GetTexture(texture_displacement, GAME_ENGINE_TEXTURE_TYPE_DISPLACEMENT_MAP);
        texture_normal_ = instance.GetTexture(texture_normal, GAME_ENGINE_TEXTURE_TYPE_NORMAL_MAP);

        size_t rendering_queue_ = 0;
    }

    void MaterialDeferredDisplacement::Render(opengl::OpenGLRenderer * renderer, opengl::OpenGLObject & object, glm::mat4 & model)
    {
        renderer->DrawDisplacement(object, model, texture_displacement_, texture_normal_, displacement_intensity_);
    }

}

}