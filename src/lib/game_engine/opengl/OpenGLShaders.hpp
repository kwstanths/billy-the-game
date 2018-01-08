#ifndef __OpenGLShaders_hpp__
#define __OpenGLShaders_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

namespace game_engine {

    static const char shader_name_vertex_position[] = "vertex_position_modelspace";
    static const char shader_name_vertex_uv[] = "vertex_uv";
    static const char shader_name_uni_model[] = "matrix_model";
    static const char shader_name_uni_view[] = "matrix_view";
    static const char shader_name_uni_projection[] = "matrix_projection";
    static const char shader_name_uni_texture[] = "sampler_texture";

    static const char shader_text_name_vertex[] = "vertex";
    static const char shader_text_name_uni_projection[] = "matrix_projection";
    static const char shader_text_name_uni_texture[] = "sampler_texture";
    static const char shader_text_name_uni_texture_color[] = "texture_color";

    typedef struct {
        GLuint program_id_;

        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;

        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
        GLuint uni_Texture_;

    } OpenGLShaderVariables_t;

    typedef struct {
        GLuint program_id_;

        GLuint attr_vertex_;

        GLuint uni_Projection_;
        GLuint uni_Texture_;
        GLuint uni_Texture_color_;
    } OpenGLShaderTextVariables_t;

    /**
        Compile and link vertex and fragment shaders. Find and set the ids for the shader variables
        @param vertex_shader_path The disk file path for the vertex shader
        @param fragment_shader_path The disk file path for the fragment shader
        @param[out] shader_vars A struct with the necessary shader variables ids
    */
    int OpenGLLoadShaders(std::string vertex_shader_path, std::string fragment_shader_path, OpenGLShaderVariables_t * shader_vars);

    /**
    
    */
    int OpenGLLoadShaders(std::string vertex_shader_path, std::string fragment_shader_path, OpenGLShaderTextVariables_t * shader_vars);

    /**
        Parse, compile and link vertex and fragment shaders
        @param vertex_file_path Disk file path for vertex_shader
        @param fragment_file_path Disk file path for fragment shader
        @param[out] program_id The compiled program id
    */
    int OpenGLCompileShaders(const char * vertex_file_path, const char * fragment_file_path, GLuint * program_id);


}

#endif