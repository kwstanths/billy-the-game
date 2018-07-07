#ifndef __OpenGLShaders_hpp__
#define __OpenGLShaders_hpp__

#include <string>

#include "glm/glm.hpp"

#include "OpenGLIncludes.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    /* Names of the main shader variables used */
    static const char shader_main_vertex_position[] = "vertex_position_modelspace";
    static const char shader_main_vertex_uv[] = "vertex_uv";
    static const char shader_main_vertex_normal[] = "vertex_normal";

    static const char shader_main_uni_model[] = "matrix_model";
    static const char shader_main_uni_view[] = "matrix_view";
    static const char shader_main_uni_projection[] = "matrix_projection";
    static const char shader_main_uni_camera_position_worldspace[] = "camera_position_worldspace";

    /* Names of the simple shader varialbes used */
    static const char shader_simple_vertex_position[] = "vertex_position_modelspace";
    static const char shader_simple_vertex_uv[] = "vertex_uv";

    static const char shader_simple_uni_model[] = "matrix_model";
    static const char shader_simple_uni_view[] = "matrix_view";
    static const char shader_simple_uni_projection[] = "matrix_projection";
    static const char shader_simple_uni_texture[] = "sampler_texture";

    /* Names of the text shader variables used */
    static const char shader_text_name_vertex[] = "vertex";
    static const char shader_text_name_uni_projection[] = "matrix_projection";
    static const char shader_text_name_uni_texture_color[] = "texture_color";
    static const char shader_text_name_uni_texture[] = "sampler_texture";

    /**
        A shader class the encapsulates shader fuctionality
    */
    class OpenGLShader {
    public:
        OpenGLShader();

        /**
            Initialize a vertex and a fragment shader, compile and link them
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);

        /**
            Destroys and deallocates. Currently does nothing
            @return -1 = Not initialised, 0 = OK
        */
        int Destroy();

        /**
            Ask wether the object is initialised
            @return true = YES, false = NO
        */
        bool IsInited();

        /**
            Opengl call to use this shader program
            @return false = not initialised, true = OK
        */
        bool Use();

        /**
            Get the location in the shader of an attribute variable
            @param attribute_name The name of the attribute
            @return -1 = Not initialised, or not found, else the location
        */
        GLint GetAttributeLocation(std::string attribute_name);

        /**
            Get the location in the shader of a uniform variable
            @param uniform_name The name of the uniform
            @return -1 = Not initialised, or not found, else the location
        */
        GLint GetUniformLocation(std::string uniform_name);

        void SetUniformMat4(GLuint id, glm::mat4 & model);

        void SetUniformVec3(GLuint id, glm::vec3 & vector);

        void SetUniformFloat(GLuint id, float value);

        void SetUniformInt(GLuint id, int value);

        void SetUniformUInt(GLuint id, unsigned int value);

    protected:
        GLuint program_id_;
    private:
        bool is_inited_;

        /**
            Compile and link a shader program
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp

        */
        int CompileShaders(std::string vertex_file_path, std::string fragment_file_path);
    };


    /**
        Shader object for the main shader
    */
    class OpenGLShaderMain : public OpenGLShader {
    public:
        /**
            Does nothing in particular. Call Init()
        */
        OpenGLShaderMain();

        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);

        /* Varialbe locations for the shader varialbes used */
        /* Attributes */
        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;
        GLuint attr_vertex_normal_;

        /* Uniforms */
        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
        GLuint uni_camera_position_worldspace_;
    };

    class OpenGLShaderText : public OpenGLShader {
    public:
        OpenGLShaderText();

        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);

        /* Varialbe locations for the shader varialbes used */
        /* Attributes */
        GLuint attr_vertex_;

        /* Uniforms */
        GLuint uni_Projection_;
        GLuint uni_Texture_;
        GLuint uni_Texture_color_;
    };

    /**
        Shader object for the simple shader
    */
    class OpenGLShaderSimple : public OpenGLShader {
    public:
        /**
            Does nothing in particular. Call Init()
        */
        OpenGLShaderSimple();

        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);

        /* Varialbe locations for the shader varialbes used */
        /* Attributes */
        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;

        /* Uniforms */
        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
        GLuint uni_texture_;
    };

}
}
}

#endif