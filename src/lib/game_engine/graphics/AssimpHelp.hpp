#ifndef __AssimpHelp_hpp__
#define __AssimpHelp_hpp__

#include <string>
#include <vector>

#include "Mesh.hpp"
#include "Material.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"


namespace game_engine {
namespace graphics {
    
    struct AssimpData_t {
        Mesh * mesh_;
        Material * material_;
        AssimpData_t(Mesh * mesh, Material * material) : mesh_(mesh), material_(material) {};
    };

    int LoadModel(std::string file_path, std::vector<AssimpData_t>& out_meshes);

    int ProcessNode(aiNode *node, const aiScene *scene, std::string directory, std::vector<AssimpData_t>& out_meshes);

    AssimpData_t ProcessMesh(aiMesh *mesh, const aiScene *scene, std::string directory);

    std::vector<Texture_t> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, int texture_type, std::string directory);

    int ProcessObjectAtlas(std::string file_path, std::vector<AssimpData_t>& out_meshes);

}
}

#endif