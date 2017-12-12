#ifndef __OpenGLShaderVariables_hpp__
#define __OpenGLShaderVariables_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

namespace game_engine {

    static const char name_vertex_position[] = "vertex_position_modelspace";
    static const char name_vertex_color[] = "vertex_color";
    static const char name_uni_model[] = "matrix_model";
    static const char name_uni_view[] = "matrix_view";
    static const char name_uni_projection[] = "matrix_projection";

    typedef struct {
        GLuint program_id_;

        GLuint attr_vertex_position_;
        GLuint attr_vertex_color_;

        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;

    } OpenGLShaderVariables_t;

}

#endif