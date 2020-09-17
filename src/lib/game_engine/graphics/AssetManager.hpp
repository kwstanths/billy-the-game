#ifndef __AssetManager_hpp__
#define __AssetManager_hpp__

#include <string>

#include "game_engine/utility/HashTable.hpp"
#include "game_engine/graphics/Model.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/opengl/OpenGLShaders.hpp"


namespace game_engine {
namespace graphics {

    extern size_t MODELS_NUMBER;
    extern size_t TEXTURES_NUMBER;

    /**
        A signleton class to manage game assets. Quick search for an asset,
        and insert new ones
    */
    class AssetManager {
    public:

        static AssetManager & GetInstance() {
            static AssetManager instance;
            return instance;
        }

        /**
            Be very carefull when destroying the asset manager, since it will delete the
            meshes themselves! Pointers to meshes may be everywhere!
        */
        ~AssetManager();

        /**
            Search for a mesh, based on the name
            @param The name of the mesh
            @return nullptr = not found, else the pointer to that object
        */
        Model * FindModel(std::string name);

        /**
            Insert a mesh. Produces a warning if already inserted
            @param name The name of the mesh
            @param mesh A pointer to the mesh allocation
        */
        void InsertModel(std::string name, Model * model);



        /**
            Search for a texture, based on a file path
            @param name The file path of the texture
            @return nullptr = not found, else a pointer to that object
        */
        opengl::OpenGLTexture * FindTexture(std::string name);

        /**
            Insert a texture. Produces a warning if already inserted
            @param name The name of the texture
            @param mesh A pointer to the texture
        */
        void InsertTexture(std::string name, opengl::OpenGLTexture * texture);

        /**
            Try to find a shader, if not found, initialize, insert, and return a pointer to it
            @param name The name of the texture
            @param type The type of the texture. Used only if initializing
            @return A pointer to the texture, nullptr if not found, and can't initialize it 
        */
        opengl::OpenGLTexture * GetTexture(std::string name, int type);

    private:
        /* Holds the models */
        utility::HashTable<std::string, Model *> * models_;
        /* Holds the textures */
        utility::HashTable<std::string, opengl::OpenGLTexture *> * textures_;

        /**
            Does nothing in particular. Call Init()
        */
        AssetManager();
    };

}
}


#endif