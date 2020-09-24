#include "AssimpHelp.hpp"

#include "game_engine/core/FileSystem.hpp"
#include "game_engine/math/Vector.hpp"

#include "Material.hpp"
#include "Mesh.hpp"

namespace math = game_engine::math;

namespace game_engine {
namespace graphics{

    int LoadModel(std::string file_path, std::vector<AssimpData_t>& out_meshes) {

        std::string directory = FileSystem::GetInstance().GetDirectoryAssets();
        std::string full_path = directory + "/" + file_path;

        /* Use assimp to load the model */
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(full_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            dt::ConsoleInfoL(dt::WARNING, "Assimp::ReadFile error", "error", importer.GetErrorString());
            return -1;
        }
    
        ProcessNode(scene->mRootNode, scene, directory, out_meshes);
    
        return 0;
    }
    
    int ProcessNode(aiNode * node, const aiScene * scene, std::string directory, std::vector<AssimpData_t>& out_meshes) {
        /* process all the node's meshes (if any) */
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    
            out_meshes.push_back(ProcessMesh(mesh, scene, directory));
        }
        /* then do the same for each of its children */
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene, directory, out_meshes);
        }
    
        return 0;
    }
    
    AssimpData_t ProcessMesh(aiMesh *mesh, const aiScene *scene, std::string directory) {
        std::vector<Vertex_t> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture_t> textures;
    
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex_t vertex;
            /* Process vertex position */
            vertex.position_ = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            /* Process vertex normal */
            vertex.normal_ = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            /* Process vertex uv coordinates */
            if (mesh->mTextureCoords[0]) {
                /* does the mesh contain texture coordinates? */
                vertex.uv_ = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else
                vertex.uv_ = glm::vec2(0.0f, 0.0f);
    
    
            vertices.push_back(vertex);
        }
        /* Process indices */
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
    
        /* Process materials */
        aiColor3D color_ambient;
        aiColor3D color_diffuse;
        aiColor3D color_specular;
        Real_t shininess;
        std::vector<Texture_t> diffuseMaps;
        std::vector<Texture_t> specularMaps;
        std::vector<Texture_t> normalMaps;
        std::vector<Texture_t> displMaps;

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            
            material->Get(AI_MATKEY_COLOR_AMBIENT, color_ambient);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color_diffuse);
            material->Get(AI_MATKEY_COLOR_SPECULAR, color_specular);
            material->Get(AI_MATKEY_SHININESS, shininess);
    
            diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP, directory);
            if (diffuseMaps.size() == 0) diffuseMaps.push_back(Texture_t(directory + std::string("/textures/spec_map_empty.png"), GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP));
    
            specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP, directory);
            if (specularMaps.size() == 0) specularMaps.push_back(Texture_t(directory + std::string("/textures/spec_map_empty.png"), GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP));

            /* Not imported currently. They have to be set explicitly when seting a material for a mesh */
            normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, GAME_ENGINE_TEXTURE_TYPE_NORMAL_MAP, directory);
            displMaps = LoadMaterialTextures(material, aiTextureType_DISPLACEMENT, GAME_ENGINE_TEXTURE_TYPE_DISPLACEMENT_MAP, directory);
        }
           
        /* Create a default material */
        math::Vector3D diffuse_color(color_diffuse.r, color_diffuse.g, color_diffuse.b);
        math::Vector3D specular_color(color_specular.r, color_specular.g, color_specular.b);
        MaterialDeferredStandard * material_default = new MaterialDeferredStandard(diffuse_color, specular_color, diffuseMaps[0].path_, specularMaps[0].path_);

        Mesh * temp_mesh = new Mesh();
        temp_mesh->Init(vertices, indices);
    
        return AssimpData_t(temp_mesh, material_default);
    }
    
    std::vector<Texture_t> LoadMaterialTextures(aiMaterial * mat, aiTextureType type, int texture_type, std::string directory) {
    
        std::vector<Texture_t> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture_t texture;
            texture.path_ = directory + "/" + std::string(str.C_Str());
            texture.type_ = texture_type;
            textures.push_back(texture);
        }
        return textures;
    
    }

    int ProcessObjectAtlas(std::string file_path, std::vector<AssimpData_t>& out_meshes) {

        std::string directory = FileSystem::GetInstance().GetDirectoryAssets();
        std::string full_path = directory + "/" + file_path;

        /* Use assimp to load the model */
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(full_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            dt::ConsoleInfoL(dt::WARNING, "Assimp::ReadFile error", "error", importer.GetErrorString());
            return -1;
        }

        ProcessNode(scene->mRootNode, scene, directory, out_meshes);

        return 0;
    }

}
}
