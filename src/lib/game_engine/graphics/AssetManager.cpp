#include "AssetManager.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace utl = game_engine::utility;
namespace gl = game_engine::graphics::opengl;

namespace game_engine {
namespace graphics {

    size_t MODELS_NUMBER = 512;
    size_t TEXTURES_NUMBER = 512;

    AssetManager::AssetManager() {

        models_ = new utl::HashTable<std::string, Model *>(MODELS_NUMBER, 1.0f);
        textures_ = new utl::HashTable<std::string, gl::OpenGLTexture *>(TEXTURES_NUMBER, 1.0);
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
            dt::Console(dt::WARNING, "AssetManager::InsertTexure(): Texture already present");

    }

}
}
