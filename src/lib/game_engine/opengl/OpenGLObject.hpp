#ifndef __OpenGLObject_hpp__
#define __OpenGLObject_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLShaderVariables.hpp"
#include "OpenGLContext.hpp"

#include <glm/glm.hpp>

namespace game_engine {

    /**
        Values necessary to initialize the OpenGLObject
    */
    typedef struct {
        GLfloat pos_x_, pos_y_, pos_z_;
        std::string object_path_;
        std::string texture_path_;
    } OpenGLObjectConfig_t;


    class OpenGLObject {
    public:
        /**
            Does nothing
        */
        OpenGLObject();

        /**
            Reads object and texture files, initializes vertex array object
        */
        int Init(OpenGLObjectConfig_t config, OpenGLContext * context);

        /**
            Deallocates everything
        */
        int Destroy();

        /**
            Paint the object
        */
        int Draw(double delta_time);

        /**
            Return wether the object is initialised
        */
        bool IsInited();

    protected:
        /**
            Set rendered object position
        */
        void SetPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z);

        GLfloat pos_x_, pos_y_, pos_z_;

    private:
        bool is_inited_;
        glm::mat4 model_;
        OpenGLShaderVariables_t shader_vars_;

        size_t triangles_;
        GLuint VAO_, vertex_buffer_, uv_buffer_, texture_;

    };

}

#endif