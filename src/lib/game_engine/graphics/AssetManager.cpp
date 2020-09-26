#include "AssetManager.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace utl = game_engine::utility;
namespace gl = game_engine::graphics::opengl;

namespace game_engine {
namespace graphics {

    size_t MODELS_NUMBER = 512;
    size_t TEXTURES_NUMBER = 512;
    size_t MATERIALS_NUMBER = 512;

    AssetManager::AssetManager() {

        models_ = new utl::HashTable<std::string, Model *>(MODELS_NUMBER, 1.0f);
        textures_ = new utl::HashTable<std::string, gl::OpenGLTexture *>(TEXTURES_NUMBER, 1.0);
        materials_ = new utl::HashTable<std::string, Material *>(MATERIALS_NUMBER, 1.0);
    }

    AssetManager::~AssetManager() {
        /* Destroy models */
        for (typename utl::HashTable<std::string, Model *>::iterator itr = models_->begin(); itr != models_->end(); ++itr) {
            Model * model = itr.GetValue();
            model->Destroy();
            delete model;
        }
        delete models_;

        /* Destroy textures */
        for (typename utl::HashTable<std::string, gl::OpenGLTexture *>::iterator itr = textures_->begin(); itr != textures_->end(); ++itr) {
            gl::OpenGLTexture * texture = itr.GetValue();
            texture->Destroy();
            delete texture;
        }
        delete textures_;

    }

    Model * AssetManager::FindModel(std::string name) {

        utl::HashTable<std::string, Model *>::iterator itr = models_->Find(name);
        if (itr != models_->end())
            return itr.GetValue();

        return nullptr;
    }

    void AssetManager::InsertModel(std::string name, Model * model) {

        bool ret = models_->Insert(name, model);
        if (!ret)
            dt::Console(dt::WARNING, "AssetManager::InsertModel(): Model already present");

    }

    opengl::OpenGLTexture * AssetManager::FindTexture(std::string name) {

        utl::HashTable<std::string, gl::OpenGLTexture *>::iterator itr = textures_->Find(name);
        if (itr != textures_->end())
            return itr.GetValue();

        return nullptr;
    }

    void AssetManager::InsertTexture(std::string name, opengl::OpenGLTexture * texture) {

        bool ret = textures_->Insert(name, texture);
        if (!ret)
            dt::Console(dt::WARNING, "AssetManager::InsertTexture(): Texture already present");

    }

    opengl::OpenGLTexture * AssetManager::GetTexture(std::string name, int type)
    {
        
        /* Try to find if it was previously inserted */
        gl::OpenGLTexture * previously_allocated_texture = FindTexture(name);

        /* Else initialize it and insert it */
        int ret = 0;
        if (!previously_allocated_texture) {
            previously_allocated_texture = new gl::OpenGLTexture();
            ret = previously_allocated_texture->Init(name, type);
        
            if (!ret) InsertTexture(name, previously_allocated_texture);
            else {
                dt::Console(dt::WARNING, "Can't find texture: " + name);
            }
        }

        if (!ret) return previously_allocated_texture;
        else return nullptr;

    }

    Material * AssetManager::FindMaterial(std::string name) {
        utl::HashTable<std::string, Material *>::iterator itr = materials_->Find(name);
        if (itr != materials_->end())
            return itr.GetValue();

        return nullptr;
    }

    void AssetManager::InsertMaterial(std::string name, Material * material)
    {
        bool ret = materials_->Insert(name, material);
        if (!ret)
            dt::Console(dt::WARNING, "AssetManager::InsertMaterial(): Material already present");
    }

}
}
