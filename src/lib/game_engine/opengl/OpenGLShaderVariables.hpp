#ifndef __OpenGLShaderVariables_hpp__
#define __OpenGLShaderVariables_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

namespace game_engine {

    static const char shader_name_vertex_position[] = "vertex_position_modelspace";
    static const char shader_name_vertex_uv[] = "vertex_uv";
    static const char shader_name_uni_model[] = "matrix_model";
    static const char shader_name_uni_view[] = "matrix_view";
    static const char shader_name_uni_projection[] = "matrix_projection";
    static const char shader_name_uni_texture[] = "sampler_texture";

    typedef struct {
        GLuint program_id_;

        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;

        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
        GLuint uni_Texture_;

    } OpenGLShaderVariables_t;

}

#endif