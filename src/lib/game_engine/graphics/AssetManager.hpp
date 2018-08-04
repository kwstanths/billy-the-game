#ifndef __AssetManager_hpp__
#define __AssetManager_hpp__

#include <string>

#include "game_engine/graphics/Mesh.hpp"
#include "game_engine/utility/HashTable.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"


namespace game_engine {
namespace graphics {

    extern size_t MESHES_NUMBER;
    extern size_t TEXTURES_NUMBER;

    /**
        Object to manage assets to only keep one copy for each and easy
        searching among them
        TODO Sequential memory allocation via the memory_subsystem
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
        Mesh * FindMesh(std::string name);

        /**
            Insert a mesh. Produces a warning if already inserted
            @param name The name of the mesh
            @param mesh A pointer to the mesh allocation
        */
        void InsertMesh(std::string name, Mesh * mesh);

        /**
        
        */
        opengl::OpenGLTexture * FindTexture(std::string name);

        /**
        
        */
        void InsertTexture(std::string name, opengl::OpenGLTexture * texture);

    private:
        /* Holds the meshes */
        utility::HashTable<std::string, Mesh *> * meshes_;
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