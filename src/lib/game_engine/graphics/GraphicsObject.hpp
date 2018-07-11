#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__

#include <assimp/scene.h>

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"

#include "GraphicsTypes.hpp"
#include "Mesh.hpp"

namespace game_engine {
namespace graphics {
    
    class Renderer;

    class GraphicsObject {
        friend class Renderer;
    public:
        GraphicsObject();

        int Init(float x, float y, float z, std::string model_file_path);

        int Destroy();

        bool IsInited();

        virtual void Draw(Renderer * renderer);

        void SetPosition(float x, float y, float z);

        void Scale(float scale_x, float scale_y, float scale_z);

        void Rotate(float angle, size_t axis);

        void SetModelMatrix();

    private:
        bool is_inited_;

        std::vector<Mesh> meshes_;
        std::string directory_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_;

        int LoadModel(std::string file_path);

        int ProcessNode(aiNode *node, const aiScene *scene);

        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture_t> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, int texture_type);
    };

}
}

#endif