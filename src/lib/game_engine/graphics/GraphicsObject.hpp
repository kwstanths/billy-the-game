#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__

#include <assimp/scene.h>

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/math/Types.hpp"

#include "GraphicsTypes.hpp"
#include "Mesh.hpp"

namespace game_engine {
namespace graphics {
    
    class Renderer;

    class GraphicsObject {
        friend class Renderer;
    public:
        GraphicsObject();

        virtual ~GraphicsObject() {};

        int Init(Real_t x, Real_t y, Real_t z, std::string model_file_path);

        int Destroy();

        bool IsInited();

        virtual void Draw(Renderer * renderer);

        void SetPosition(Real_t x, Real_t y, Real_t z);

        void Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z);

        /**
            Set the rotation matrix of the model.
        */
        void SetRotation(Real_t angle, glm::vec3 axis);

        /** 
            Rotate the model around the axis, takes in mind previous rotations
        */
        void Rotate(Real_t angle, glm::vec3 axis);

        void SetModelMatrix();

    private:
        bool is_inited_;

        std::vector<Mesh *> meshes_;
        std::string asset_file_path;
        std::string directory_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_;

        int LoadModel(std::string file_path);

        int ProcessNode(aiNode *node, const aiScene *scene);

        Mesh * ProcessMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture_t> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, int texture_type);
    };

}
}

#endif