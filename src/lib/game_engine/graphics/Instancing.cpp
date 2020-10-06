#include "Instancing.hpp"


namespace game_engine { namespace graphics {

    Instancing::Instancing() {
        is_inited_ = false;

    }

    int Instancing::Init() {

        instances_ = new utility::HashTable<Mesh *, utility::HashTable<Material *, std::vector<glm::mat4>> *>(50, 1);

        is_inited_ = true;
        return 0;
    }

    void Instancing::AddInstance(Mesh * mesh, Material * material, glm::mat4& model_matrix) {
        if (!is_inited_) return;

        auto mesh_itr = instances_->Find(mesh);
        if (mesh_itr != instances_->end()) {
            utility::HashTable<Material *, std::vector<glm::mat4>> * materials = mesh_itr.GetValue();
            auto material_itr = materials->Find(material);
            if (material_itr != materials->end()) {
                material_itr.GetValue().push_back(model_matrix);
            } else {
                materials->Insert(material, { model_matrix });
            }
        } else {
            utility::HashTable<Material *, std::vector<glm::mat4>> * temp = new utility::HashTable<Material *, std::vector<glm::mat4>>(50, 1);
            temp->Insert(material, { model_matrix });
            instances_->Insert(mesh, temp);
        }

    }

    void Instancing::PrepareBuffers()
    {
        for (auto mesh_itr = instances_->begin(); mesh_itr != instances_->end(); ++mesh_itr) {
            utility::HashTable<Material *, std::vector<glm::mat4>> * materials = mesh_itr.GetValue();
            for (auto mat_itr = materials->begin(); mat_itr != materials->end(); ++mat_itr) {
                Mesh * mesh = mesh_itr.GetKey();
                Material * material = mat_itr.GetKey();
                std::vector<glm::mat4> models = mat_itr.GetValue();

                unsigned int models_buffer;
                glGenBuffers(1, &models_buffer);
                glBindBuffer(GL_ARRAY_BUFFER, models_buffer);
                glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

                InstanceDrawCall data;
                data.amount_ = models.size();
                data.material_ = material;
                data.mesh_ = mesh;
                data.buffer_ = models_buffer;
                instanced_draws_.push_back(data);
            }
        }

        buffers_prepared_ = true;
    }

}
}