#ifndef __AssetManager_hpp__
#define __AssetManager_hpp__

#include <string>

#include "game_engine/utility/HashTable.hpp"
#include "game_engine/graphics/Model.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"


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
        
        */
        opengl::OpenGLTexture * FindTexture(std::string name);

        /**
        
        */
        void InsertTexture(std::string name, opengl::OpenGLTexture * texture);

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