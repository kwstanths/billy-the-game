#ifndef __OpenGLShadowMap_hpp__
#define __OpenGLShadowMap_hpp__

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLContext.hpp"
#include "OpenGLCamera.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLShadowMap {
    public:
        OpenGLShadowMap();

        ~OpenGLShadowMap();

        int Init(size_t width, size_t height);

        int Destroy();

        int Bind(size_t index);
        int Unbind();

        void CalculateProjectionMatrices(glm::vec3 light_direction, OpenGLCamera * camera, glm::mat4& light_space);

        int ActivateTextures(size_t index);

        void ConfigureViewport();

        void ClearDepth();

        size_t GetNCascades();

        glm::mat4& GetProjectionMatrix(size_t index);

        GLuint shadow_maps[3];
    private:
        bool is_inited_;
        size_t width_;
        size_t height_;

        GLuint frame_buffer_;

        size_t n_shadow_maps_ = 3;
        glm::mat4 projection_matrices[3];
    };

}
}
}

#endif