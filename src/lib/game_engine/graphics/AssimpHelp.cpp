#include "AssimpHelp.hpp"

#include "GraphicsTypes.hpp"

namespace game_engine {
namespace graphics{

    int LoadModel(std::string file_path, std::vector<Mesh *>& out_meshes) {

        /* Use assimp to load the model */
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            dt::ConsoleInfoL(dt::WARNING, "Assimp::ReadFile error", "error", importer.GetErrorString());
            return -1;
        }
    
        std::string directory = file_path.substr(0, file_path.find_last_of('/'));
    
        ProcessNode(scene->mRootNode, scene, directory, out_meshes);
    
        return 0;
    }
    
    int ProcessNode(aiNode * node, const aiScene * scene, std::string directory, std::vector<Mesh *>& out_meshes) {
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
    
    Mesh * ProcessMesh(aiMesh *mesh, const aiScene *scene, std::string directory) {
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
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    
            material->Get(AI_MATKEY_COLOR_AMBIENT, color_ambient);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color_diffuse);
            material->Get(AI_MATKEY_COLOR_SPECULAR, color_specular);
            material->Get(AI_MATKEY_SHININESS, shininess);
    
            std::vector<Texture_t> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP, directory);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            if (textures.size() == 0) textures.push_back(Texture_t(std::string("assets/textures/spec_map_empty.png"), GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP));
    
            std::vector<Texture_t> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP, directory);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            if (textures.size() == 1) textures.push_back(Texture_t(std::string("assets/textures/spec_map_empty.png"), GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP));
        }
    
        Mesh * temp_mesh = new Mesh();
        temp_mesh->Init(vertices, indices, textures, Material_t(shininess,
            glm::vec3(color_ambient.r, color_ambient.g, color_ambient.b),
            glm::vec3(color_diffuse.r, color_diffuse.g, color_diffuse.b),
            glm::vec3(color_specular.r, color_specular.g, color_specular.b))
        );
    
        return temp_mesh;
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

    int ProcessTextureAtlas(std::string file_path, std::vector<Mesh*>& out_meshes) {

        /* Use assimp to load the model */
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            dt::ConsoleInfoL(dt::WARNING, "Assimp::ReadFile error", "error", importer.GetErrorString());
            return -1;
        }

        std::string directory = file_path.substr(0, file_path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene, directory, out_meshes);

        return 0;
    }

}
}