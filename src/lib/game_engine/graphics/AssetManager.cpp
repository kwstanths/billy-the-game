#include "AssetManager.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace utl = game_engine::utility;
namespace gl = game_engine::graphics::opengl;

namespace game_engine {
namespace graphics {

    AssetManager::AssetManager() {

        meshes_ = new utl::HashTable<std::string, Mesh *>(512, 1.0);
        textures_ = new utl::HashTable<std::string, gl::OpenGLTexture *>(512, 1.0);
    }

    AssetManager::~AssetManager() {
        /* Destroy meshes */
        for (typename utl::HashTable<std::string, Mesh *>::iterator itr = meshes_->begin(); itr != meshes_->end(); ++itr) {
            Mesh * mesh = itr.GetValue();
            mesh->Destroy();
            delete mesh;
        }
        delete meshes_;

        /* Destroy textures */
        for (typename utl::HashTable<std::string, gl::OpenGLTexture *>::iterator itr = textures_->begin(); itr != textures_->end(); ++itr) {
            gl::OpenGLTexture * texture = itr.GetValue();
            texture->Destroy();
            delete texture;
        }
        delete textures_;

    }

    Mesh * AssetManager::FindMesh(std::string name) {

        utl::HashTable<std::string, Mesh *>::iterator itr = meshes_->Find(name);
        if (itr != meshes_->end())
            return itr.GetValue();

        return nullptr;
    }

    void AssetManager::InsertMesh(std::string name, Mesh * mesh) {

        bool ret = meshes_->Insert(name, mesh);
        if (!ret)
            dt::Console(dt::WARNING, "AssetManager::InsertMesh(): Mesh already present");

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
