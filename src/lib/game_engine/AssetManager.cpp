#include "AssetManager.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

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

    OpenGLObject * AssetManager::FindObject(std::string object_name, int * ret_code) {
        if (!IsInited()) {
            *ret_code = Error::ERROR_GEN_NOT_INIT;
            return nullptr;
        }

        /* Search among the already created objects */
        utl::HashTable<std::string, OpenGLObject *>::iterator itr = objects_->Find(object_name);
        if (itr != objects_->end()) {
            *ret_code = Error::ERROR_NO_ERROR;
             return itr.GetValue();
        }

        /* If not found, create it and insert it in the table */
        OpenGLObject * object = new OpenGLObject();
        int ret = object->Init(object_name);
        if (ret != 0) {
            delete object;
            *ret_code = ret;

            dt::ConsoleInfoL(dt::CRITICAL, "OpenGL object not found",
                "name", object_name);

            return nullptr;
        }
        
        objects_->Insert(object_name, object);

        *ret_code = 0;
        return object;
    }

    OpenGLTexture * AssetManager::FindTexture(std::string texture_name, OpenGLTexture::OpenGLTextureType type, int * ret_code) {
        if (!IsInited()) {
            *ret_code = Error::ERROR_GEN_NOT_INIT;
            PrintError(*ret_code);
            return nullptr;
        }

        /* Search among the already created objects */
        utl::HashTable<std::string, OpenGLTexture *>::iterator itr = textures_->Find(texture_name);
        if (itr != textures_->end()) {
            *ret_code = Error::ERROR_NO_ERROR;
            return itr.GetValue();
        }

        /* If not found, create it and insert it in the table */
        OpenGLTexture * texture = new OpenGLTexture();
        int ret = texture->Init(texture_name, type);
        if (ret != 0) {
            delete texture;
            *ret_code = ret;

            dt::ConsoleInfoL(dt::CRITICAL, "OpenGL Texture not found", 
                "name", texture_name,
                "type", type);

            return nullptr;
        }

        textures_->Insert(texture_name, texture);
        
        *ret_code = Error::ERROR_NO_ERROR;
        return texture;
    }


}
