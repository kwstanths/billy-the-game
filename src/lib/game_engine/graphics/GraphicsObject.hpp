#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__

#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"

#include "Material.hpp"

namespace game_engine {
namespace graphics {
    
    class Renderer;

    class GraphicsObject {
        friend class Renderer;
    public:
        GraphicsObject();

        int Init(float x, float y, float z, opengl::OpenGLObject * object, opengl::OpenGLTexture * texture);

        int Destroy();

        bool IsInited();

        virtual void Draw(Renderer * renderer);

        void SetPosition(float x, float y, float z);

        void Scale(float scale_x, float scale_y, float scale_z);

        void Rotate(float angle, size_t axis);

        void SetMaterial(Material_t mtl);

    private:
        bool is_inited_;

        opengl::OpenGLObject * object_;
        opengl::OpenGLTexture * texture_;
        Material_t object_material_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 scale_matrix_;
        glm::mat4 model_;
    };

}
}

#endif