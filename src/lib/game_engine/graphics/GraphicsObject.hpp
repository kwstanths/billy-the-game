#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__


#include "game_engine/math/Types.hpp"

#include "GraphicsTypes.hpp"
#include "Model.hpp"

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

        Model * model_ = nullptr;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_matrix_;
    };

}
}

#endif