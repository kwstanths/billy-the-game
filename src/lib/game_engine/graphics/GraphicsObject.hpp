#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__

#include "cal3d/coremodel.h"
#include "cal3d/model.h"

#include "game_engine/math/Types.hpp"
#include "game_engine/graphics/opengl/OpenGLQuery.hpp"
#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"

#include "GraphicsTypes.hpp"
#include "Model.hpp"

namespace game_engine {
namespace graphics {
    
    class Renderer;

    class GraphicsObject {
        friend class Renderer;
    public:
        GraphicsObject();

        enum Type {
            ASSIMP,
            ANIMATED,
        };

        virtual ~GraphicsObject() {};

        /**
            Initialize the object
            @param x Position x coordinate
            @param y Position y coordinate
            @param z Position z coordinate
            @param model_file_path A disk path of the asset
        */
        int Init(Real_t x, Real_t y, Real_t z, std::string model_file_path, Type type = ASSIMP);

        static int InitTextureAtlas(std::string file_name);

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
            Rotate the model around the axis, takes in mind previous rotations
        */
        void Rotate(Real_t angle, glm::vec3 axis);

        void UpdateAnimation(float delta);

        void ExecuteAction();

        void StopAction();

    private:
        bool is_inited_;
        Type type_;

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

        const int ANIM_STATE_IDLE = 0;
        const int ANIM_STATE_FANCY = 1;
        const int ANIM_STATE_MOTION = 2;
        int m_state;
        cal3d::CalCoreModel* m_calCoreModel;
        cal3d::CalModel* m_calModel;
        int m_animationId[16];
        int m_animationCount;
        int m_meshId[32];
        int m_meshCount;
        std::vector<opengl::OpenGLTexture *> textures_;
        std::vector<opengl::OpenGLObject *> objects_;
        int m_textureCount;
        float m_motionBlend[3];
        float m_renderScale;
        float m_lodLevel;
        std::string m_path;
    };

}
}

#endif