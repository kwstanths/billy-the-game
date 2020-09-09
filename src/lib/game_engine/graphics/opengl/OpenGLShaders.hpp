#ifndef __OpenGLShaders_hpp__
#define __OpenGLShaders_hpp__

#include <string>

#include "glm/glm.hpp"

#include "OpenGLIncludes.hpp"

namespace game_engine { namespace graphics { namespace opengl {

    /* Shader variables */
    static const char shader_vertex_position[] = "vertex_position_modelspace";
    static const char shader_vertex_uv[] = "vertex_uv";
    static const char shader_vertex_normal[] = "vertex_normal";
    
    static const char shader_uni_model[] = "matrix_model";
    static const char shader_uni_view[] = "matrix_view";
    static const char shader_uni_projection[] = "matrix_projection";
    static const char shader_uni_shadow_map[] = "shadow_map";
    static const char shader_sampler_texture[] = "sampler_texture";
    static const char shader_blur_kernel_size[] = "blur_kernel_size";
    
    /* Names of the text shader variables used */
    static const char shader_text_name_vertex[] = "vertex";
    static const char shader_text_name_uni_texture_color[] = "texture_color";
    static const char shader_text_name_uni_texture[] = "sampler_texture";
    
    /* Names of the vertices color shader variables used */
    static const char shader_vertices_color_uni_color[] = "fragment_color";
    
    /* Names of the g buffer shader variables used*/
    static const char shader_gbuffer_position[] = "g_position";
    static const char shader_gbuffer_normal[] = "g_normal";
    static const char shader_gbuffer_albedo_spec[] = "g_albedo_spec";
    static const char shader_gbuffer_position_light[] = "g_position_light";
    
    /* Names of the SSAO shader variables used */
    static const char shader_ssao_noise_texture[] = "noise_texture";
    static const char shader_ssao_radius[] = "radius";
    static const char shader_ssao_samples_size[] = "samples_size";
    static const char shader_ssao_intensity[] = "intensity";
    static const char shader_ssao_bias[] = "bias";
    
    /* Names for the final pass shader variables used */
    static const char shader_final_pass_ssao_texture[] = "ssao_texture";
    
    /* Names for the shadow map shader */
    static const char shader_uni_lightspace[] = "matrix_lightspace";
    
    /**
        A shader class the encapsulates all shader fuctionality
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
            Initialize a vertex, a fragment and a geometry shader, compile and link them
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path);
    
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
    
        void SetUniformBool(GLuint id, bool value);
    
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
        int CompileShaders(std::string vertex_file_path, std::string fragment_file_path, std::string geometry_shader_path);

        int CompileShader(std::string file_path, GLuint type, int& ret);
    };
    
    /* Shader classes for specific shaders and their variables */
    
    /* Text shader */
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

    /* 3D Text shader */
    class OpenGLShaderText3D : public OpenGLShader {
    public:
        OpenGLShaderText3D();

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
        GLuint uni_Texture_;
        GLuint uni_Texture_color_;
    };
    
    /**
        Shader for the simple vertex color forward pass 
    */
    class OpenGLShaderVerticesColor : public OpenGLShader {
    public:
        /**
            Does nothing in particular. Call Init()
        */
        OpenGLShaderVerticesColor();
    
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
    
        /* Uniforms */
        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
        GLuint uni_fragment_color_;
    };
    
    /**
        Shader for Gbuffer filling pass
    */
    class OpenGLShaderGBuffer : public OpenGLShader {
    public:
        OpenGLShaderGBuffer();
    
        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);
    
        /* Attributes */
        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;
        GLuint attr_vertex_normal_;
    
        /* Uniforms */
        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
        GLuint uni_Lightspace_;
    };
    
    /* Shader for shadow map pass */
    class OpenGLShaderShadowMap : public OpenGLShader {
    public:
        OpenGLShaderShadowMap();
    
        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);
    
        /* Attributes */
        GLuint attr_vertex_position_;
        
        /* Uniforms */
        GLuint uni_Model_;
        GLuint uni_Lightspace_;
    };
    
    /* AO shader */
    class OpenGLShaderSSAO : public OpenGLShader {
    public:
        OpenGLShaderSSAO();
    
        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);
    
        /* Attributes */
        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;
    
        /* Uniforms */
        GLuint uni_g_position_;
        GLuint uni_g_normal_;
        GLuint uni_noise_texture_;
        GLuint uni_radius_;
        GLuint uni_samples_size_;
        GLuint uni_intensity_;
        GLuint uni_bias_;
        GLuint uni_matrix_projection_;
    };
    
    /* Separable AO shader */
    class OpenGLShaderSeparableAO : public OpenGLShader {
    public:
        OpenGLShaderSeparableAO();
    
        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);
    
        /* Attributes */
        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;
    
        /* Uniforms */
        GLuint uni_g_position_;
        GLuint uni_g_normal_;
        GLuint uni_noise_texture_;
        GLuint uni_radius_;
        GLuint uni_samples_size_;
        GLuint uni_intensity_;
        GLuint uni_bias_;
        GLuint uni_matrix_projection_;
    };
    
    /* Final pass shader */
    class OpenGLShaderFinalPass : public OpenGLShader {
    public:
        OpenGLShaderFinalPass();
    
        /**
            Initialize a vertex and a fragment shader, compile and link them. Initialize the variable locations
            @param vertex_shader_path The path to a vertex shader file
            @param fragment_shader_path The path to a fragment shader file
            @return -1 = Already initialised, 0 = OK, else see ErrorCodes.hpp
        */
        int Init(std::string vertex_shader_path, std::string fragment_shader_path);
    
        /* Attributes */
        GLuint attr_vertex_position_;
        GLuint attr_vertex_uv_;
    
        /* Uniforms */
        GLuint uni_texture_gbuffer_position_;
        GLuint uni_texture_gbuffer_normal_;
        GLuint uni_texture_gbuffer_albedo_spec_;
        GLuint uni_texture_gbuffer_position_light_;
        GLuint uni_texture_ssao_;
        GLuint uni_shadow_map_;
        GLuint uni_matrix_view_;
        GLuint uni_matrix_projection_;
    };

    /* A shader to draw the normals of a model */
    class OpenGLShaderDrawNormals : public OpenGLShader {
    public:
        OpenGLShaderDrawNormals();

        int Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path);

        GLuint attr_vertex_position_;
        GLuint attr_vertex_normal_;

        GLuint uni_Model_;
        GLuint uni_View_;
        GLuint uni_Projection_;
    };

    class OpenGLShaderWater : public OpenGLShader {
    public:
        OpenGLShaderWater();

        int Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path);

        GLuint attr_vertex_position_;
        GLuint attr_vertex_color_;
    };
}
}
}

#endif