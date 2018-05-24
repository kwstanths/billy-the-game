#include "AssetManager.hpp"

namespace game_engine {

    AssetManager::AssetManager() {
        is_inited_ = false;
    }

    AssetManager::~AssetManager() {
        Destroy();
    }

    bool AssetManager::Init(size_t number_of_objects, size_t number_of_textures) {
        if (IsInited()) return false;

        objects_ = new utl::HashTable<std::string, OpenGLObject *>(number_of_objects, number_of_objects);
        textures_ = new utl::HashTable<std::string, OpenGLTexture *>(number_of_textures, number_of_textures);

        is_inited_ = true;
        return true;
    }

    bool AssetManager::Destroy() {
        if (!IsInited()) return false;
    
        /* Destroy objects */
        for(typename utl::HashTable<std::string, OpenGLObject *>::iterator itr = objects_->begin(); itr != objects_->end(); ++itr){
            itr.GetValue()->Destroy();
        }
        objects_->Clear();

        /* Destroy textures */
        for(typename utl::HashTable<std::string, OpenGLTexture *>::iterator itr = textures_->begin(); itr != textures_->end(); ++itr){
            itr.GetValue()->Destroy();
        }
        textures_->Clear();

        is_inited_ = false;
        return true;
    }

    bool AssetManager::IsInited() {
        return is_inited_;
    }

    int AssetManager::GetObject(std::string object_name, OpenGLObject * object) {
        if (!IsInited()) return Error::ERROR_GEN_NOT_INIT;

        /* Search among the already created objects */
        utl::HashTable<std::string, OpenGLObject *>::iterator itr = objects_->Find(object_name);
        if (itr != objects_->end()) {
            object = itr.GetValue();
            return Error::ERROR_NO_ERROR;
        }

        /* If not found, create it and insert it in the table */
        object = new OpenGLObject();
        int ret = object->Init(object_name);
        if (ret != 0) {
            delete object;
            return ret;
        }
        
        objects_->Insert(object_name, object);
        return Error::ERROR_NO_ERROR;
    }

    int AssetManager::GetTexture(std::string texture_name, OpenGLTexture::OpenGLTextureType type, OpenGLTexture * texture) {
        if (!IsInited()) return Error::ERROR_GEN_NOT_INIT;

        /* Search among the already created objects */
        utl::HashTable<std::string, OpenGLTexture *>::iterator itr = textures_->Find(texture_name);
        if (itr != textures_->end()) {
            texture = itr.GetValue();
            return Error::ERROR_NO_ERROR;
        }

        /* If not found, create it and insert it in the table */
        texture = new OpenGLTexture();
        int ret = texture->Init(texture_name, type);
        if (ret != 0) {
            delete texture;
            return ret;
        }

        textures_->Insert(texture_name, texture);
        return Error::ERROR_NO_ERROR;
    }


}
