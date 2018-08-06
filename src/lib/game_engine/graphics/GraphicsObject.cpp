#include "GraphicsObject.hpp"
#include "Renderer.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "game_engine/ErrorCodes.hpp"
#include "game_engine/math/Matrices.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "AssetManager.hpp"

namespace math = game_engine::math;

namespace game_engine {
namespace graphics {

    GraphicsObject::GraphicsObject() {

        translation_matrix_ = math::GetTranslateMatrix(0, 0, 0);

        is_inited_ = false;
    }

    int GraphicsObject::Init(Real_t x, Real_t y, Real_t z, std::string model_file_path) {

        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
        scale_matrix_ = math::GetScaleMatrix(1.0f, 1.0f, 1.0f);

        int ret = LoadModel(model_file_path);
        if (ret) return ret;

        is_inited_ = true;
        return 0;
    }

    int GraphicsObject::Destroy() {


        is_inited_ = false;
        return 0;
    }

    bool GraphicsObject::IsInited() {
        return is_inited_;
    }

    void GraphicsObject::Draw(Renderer * renderer) {

        SetModelMatrix();
        renderer->Draw(this);
    }

    void GraphicsObject::SetPosition(Real_t x, Real_t y, Real_t z) {
        /* Set the translation matrix */
        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
    }

    void GraphicsObject::Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z) {
        /* Set the scaling matrix */
        scale_matrix_ = math::GetScaleMatrix(scale_x, scale_y, scale_z);
    }

    void GraphicsObject::SetRotation(Real_t angle, glm::vec3 axis) {
        rotation_matrix_ = math::GetRotateMatrix(angle, axis.x, axis.y, axis.z);
    }

    void GraphicsObject::Rotate(Real_t angle, glm::vec3 axis) {
        rotation_matrix_ *= math::GetRotateMatrix(angle, axis.x, axis.y, axis.z);
    }

    void GraphicsObject::SetModelMatrix() {
        model_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    }

    int GraphicsObject::LoadModel(std::string file_path) {
        
        /* Use assimp to load the model */
        Assimp::Importer importer;
        const aiScene * scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            dt::ConsoleInfoL(dt::WARNING, "Assimp::ReadFile error", "error", importer.GetErrorString());
            return -1;
        }

        directory_ = file_path.substr(0, file_path.find_last_of('/'));
        asset_file_path = file_path.substr(0, file_path.find_last_of('.'));

        ProcessNode(scene->mRootNode, scene);
        return 0;
    }

    int GraphicsObject::ProcessNode(aiNode * node, const aiScene * scene) {
        /* process all the node's meshes (if any) */
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            std::string mesh_file_path = asset_file_path + "/" + std::string(mesh->mName.C_Str());
            
            /* Grab the instance of the asset manager, and check if we already have that Mesh */
            AssetManager & asset_manager = AssetManager::GetInstance();
            Mesh * previously_allocated_mesh = asset_manager.FindMesh(mesh_file_path);
            if (!previously_allocated_mesh) {
                Mesh * new_mesh = ProcessMesh(mesh, scene);
                asset_manager.InsertMesh(mesh_file_path, new_mesh);
                meshes_.push_back(new_mesh);
            } else
                meshes_.push_back(previously_allocated_mesh);

        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }

        return 0;
    }

    Mesh * GraphicsObject::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex_t> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture_t> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex_t vertex;
            /* Process vertex position */
            vertex.position_ = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            /* Process vertex normal */
            vertex.normal_ = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            /* Provess vertex uv coordinates */
            if (mesh->mTextureCoords[0]) {
                /* does the mesh contain texture coordinates? */
                vertex.uv_ = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else
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
            
            std::vector<Texture_t> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            
            std::vector<Texture_t> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        Mesh * temp_mesh = new Mesh();
        temp_mesh->Init(vertices, indices, textures, Material_t(shininess,
            glm::vec3(color_ambient.r, color_ambient.g, color_ambient.b),
            glm::vec3(color_diffuse.r, color_diffuse.g, color_diffuse.b),
            glm::vec3(color_specular.r, color_specular.g, color_specular.b))
        );

        return temp_mesh;
    }

    std::vector<Texture_t> GraphicsObject::LoadMaterialTextures(aiMaterial * mat, aiTextureType type, int texture_type) {

        std::vector<Texture_t> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture_t texture;
            texture.path_ = directory_ + "/" + std::string(str.C_Str());
            texture.type_ = texture_type;
            textures.push_back(texture);
        }
        return textures;

    }


}
}