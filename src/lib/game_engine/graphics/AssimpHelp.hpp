#ifndef __AssimpHelp_hpp__
#define __AssimpHelp_hpp__

#include <string>
#include <vector>

#include "Mesh.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"


namespace game_engine {
namespace graphics {
    
    int LoadModel(std::string file_path, std::vector<Mesh *>& out_meshes);

    int ProcessNode(aiNode *node, const aiScene *scene, std::string directory, std::vector<Mesh *>& out_meshes);

    Mesh * ProcessMesh(aiMesh *mesh, const aiScene *scene, std::string directory);

    std::vector<Texture_t> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, int texture_type, std::string directory);


    int ProcessTextureAtlas(std::string file_path, std::vector<Mesh *>& out_meshes);

}
}

#endif