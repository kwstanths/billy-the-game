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
    } OpenGLObjectParams_t;


    class OpenGLObject {
    public:
        OpenGLObject(OpenGLObjectParams_t params);

        int Init(OpenGLContext * context_);

        int Destroy();

        //TODO delta_time needed?
        int Draw(double delta_time);

        /**
            Set position does not need to be called after Init()
        */
        void SetPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z);


    private:
        bool is_inited_;
        glm::mat4 model_;
        OpenGLObjectParams_t params_;
        OpenGLShaderVariables_t shader_vars_;

        GLuint VAO, vertex_buffer_, color_buffer_;
    };

}

#endif