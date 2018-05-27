#ifndef __AssetManager_hpp__
#define __AssetManager_hpp__

#include <string>

#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"

#include "ErrorCodes.hpp"

#include "utility/HashTable.hpp"
namespace utl = game_engine::utility;


namespace game_engine {

    /**
        Object to manage assets to only keep one copy for each and easy
        searching among them
        TODO Sequential memory allocation via the memory_subsystem
    */
    class AssetManager {
    public:
        /**
            Does nothing in particular. Call Init()
        */
        AssetManager();

        /**
            Calls Destroy()
        */
        ~AssetManager();

        /**
            Initializes the required structures 
            @param number_of_objects The number of objects to hold, roughly
            @param number_of_textures The number of textures to hold, roughly
            @return true = OK, false = NOT OK
        */
        bool Init(size_t number_of_objects, size_t number_of_textures);

        /**
            Deallocates all the objects held.
            @return true = OK, false = NOT OK
        */
        bool Destroy();

        /**
            Get wether the object is initialised already
            @return true = OK, false = NOT OK
        */
        bool IsInited();

        /**
            Search an object
            @param object_name The file path of the object
            @param[out] object A pointer to the initialised object
            @return 0 = OK, -1 = Object not initialised, else see ErrorCodes.hpp
        */
        OpenGLObject * FindObject(std::string object_name, int * ret_code);

        /**
            Search a texture
            @param texture_name The file path of the object
            @param type The type of the texture file
            @param[out] A pointer to the initialised object
            @return 0 = OK, -1 = Object not initialised, else see ErrorCodes.hpp
        */
        OpenGLTexture * FindTexture(std::string texture_name, OpenGLTexture::OpenGLTextureType type, int * ret_code);

    private:
        bool is_inited_;

        utl::HashTable<std::string, OpenGLObject *> * objects_ = nullptr;
        utl::HashTable<std::string, OpenGLTexture *> * textures_ = nullptr;
    };

}


#endif