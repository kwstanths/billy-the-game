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

    /*
        Cascaded shadow maps, with 3 cascades
    */
    class OpenGLCShadowMapS {
    public:
        OpenGLCShadowMapS();

        ~OpenGLCShadowMapS();

        /**
            @param context The OpenGLContext
            @param width The width of the shadow map textures
            @param height The height of the shadow map textures
            @return 0=OK, -1=NOT OK
        */
        int Init(OpenGLContext * context, size_t width, size_t height);

        int Destroy();

        /* Bind a cascade */
        int Bind(size_t index);
        /* Unbind */
        int Unbind();

        /**
            Calculate the view and projection matrices for each cascade. After this call, you can get the matrices
            @param light_direction The direction of the light
            @param camera The camera object
        */
        void CalculateProjectionMatrices(glm::vec3 light_direction, OpenGLCamera * camera);

        /**
            Activate all cascades for reading, at GL_TEXTURE0 + index, index+1, index+2
            @param index The start of the texture ids to activate, added to GL_TEXTURE0
        */
        void ActivateTextures(size_t index);

        /** Configure shadow map resolution viewport */
        void ConfigureViewport();

        /** Clear the depth of all shadow maps */
        void ClearDepth();

        /** Get the number of cascades used */
        size_t GetNCascades();

        /** Get projection matrix of cascade with index, starts from zero */
        glm::mat4& GetProjectionMatrix(size_t index);

        /** Get view matrix of cascade with index, starts from zero */
        glm::mat4& GetViewMatrix(size_t index);

        /** Get lightspace matrix (view * projection) of cascade with index, starts from zero */
        glm::mat4& GetLightspaceMatrix(size_t index);

        /** Get the far plane of cascade with index, starts from zero, in view space */
        float GetCascadeEnd(size_t index);

    private:
        bool is_inited_;
        size_t width_;
        size_t height_;
        float aspect_ratio_;

        GLuint frame_buffer_;

        size_t n_shadow_maps_ = 3;
        GLuint shadow_maps[3];
        glm::mat4 view_matrices_[3];
        glm::mat4 projection_matrices_[3];
        glm::mat4 lightspace_[3];
        GLfloat cascade_limits_[4];
    };

}
}
}

#endif