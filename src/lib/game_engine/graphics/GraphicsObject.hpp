#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__


#include "game_engine/math/Types.hpp"

#include "game_engine/graphics/opengl/OpenGLQuery.hpp"

#include "GraphicsTypes.hpp"
#include "Model.hpp"

namespace game_engine {
namespace graphics {
    
    class Renderer;

    /* An object that can be drawn on the screen */
    class GraphicsObject {
        friend class Renderer;
    public:
        GraphicsObject();

        virtual ~GraphicsObject() {};

        /**
            Initialize the object
            @param x Position x coordinate
            @param y Position y coordinate
            @param z Position z coordinate
            @param model_file_path A disk path of the asset
        */
        int Init(Real_t x, Real_t y, Real_t z, std::string model_file_path);

        static int InitObjectAtlas(std::string file_name);

        int Destroy();

        bool IsInited();

        virtual void Draw(Renderer * renderer);

        /**
            Set the position of the object
            @param x Position x coordinate
            @param y Position y coordinate
            @param z Position z coordinate
        */
        void SetPosition(Real_t x, Real_t y, Real_t z);

        /**
            Set the scaling of the object
            @param scale_x The x axis scaling
            @param scale_y The y axis scaling
            @param scale_z The z axis scaling
        */
        void Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z);

        /**
            Set the rotation matrix of the model.
        */
        void SetRotation(glm::mat4 rotation);

        /**
        
        */
        glm::mat4 GetRotation();

        /** 
            Rotate the model around the axis, takes in mind previous rotations
        */
        void Rotate(Real_t angle, glm::vec3 axis);

    private:
        bool is_inited_;

        Model * model_ = nullptr;

        std::vector<opengl::OpenGLQuery> mesh_queries_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_matrix_;

        /**
            Set the model matrix
        */
        void SetModelMatrix();
    };

}
}

#endif