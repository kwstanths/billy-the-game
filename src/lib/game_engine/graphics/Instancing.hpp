#ifndef __Instancing_hpp__
#define __Instancing_hpp__

#include "glm/glm.hpp"

#include "Material.hpp"
#include "Mesh.hpp"

#include "game_engine/utility/HashTable.hpp"

namespace game_engine { namespace graphics {

    class Instance;

    class Instancing {
        friend class Renderer;
    public:
        Instancing();

        int Init();

        void AddInstance(Mesh * mesh, Material * material, glm::mat4& model_matrix);

        void PrepareBuffers();

    private:
        bool is_inited_ = false;
        bool buffers_prepared_ = false;

        utility::HashTable<Mesh *, utility::HashTable<Material *, std::vector<glm::mat4>> *> * instances_;
        
        struct InstanceDrawCall {
            Material * material_;
            Mesh * mesh_;
            glm::mat4 * model_matrices_;
            GLuint model_matrices_buffer_;
            size_t amount_;
        };

        std::vector<InstanceDrawCall> instanced_draws_;
        /* list of model matrixes per instanced draw call */
        std::vector<std::vector<glm::mat4>> model_matrices_;

    };

}
}

#endif