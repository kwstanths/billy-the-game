#include "OpenGLShaders.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"
namespace dt = debug_tools;


namespace game_engine { namespace graphics { namespace opengl {

    OpenGLShader::OpenGLShader() {
        is_inited_ = false;
    }
    
    int OpenGLShader::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;
    
        int ret = CompileShaders(vertex_shader_path, fragment_shader_path, "", "", "");
    
        is_inited_ = true;
        return ret;
    }

    int OpenGLShader::Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path)
    {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        int ret = CompileShaders(vertex_shader_path, fragment_shader_path, "", "", geometry_shader_path);

        is_inited_ = true;
        return ret;
    }

    int OpenGLShader::Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string tesselation_control_shader, std::string tesselation_evaluation_shader)
    {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        int ret = CompileShaders(vertex_shader_path, fragment_shader_path, tesselation_control_shader, tesselation_evaluation_shader, "");

        is_inited_ = true;
        return ret;
    }

    int OpenGLShader::Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string tesselation_control_shader, std::string tesselation_evaluation_shader, std::string geometry_shader_path)
    {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        int ret = CompileShaders(vertex_shader_path, fragment_shader_path, tesselation_control_shader, tesselation_evaluation_shader, geometry_shader_path);

        is_inited_ = true;
        return ret;
    }
    
    int OpenGLShader::Destroy() {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
        /* TODO Properly destroy() */
    
        is_inited_ = false;
        return 0;
    }
    
    bool OpenGLShader::IsInited() {
        return is_inited_;
    }
    
    int OpenGLShader::CompileShaders(std::string vertex_file_path, std::string fragment_file_path, std::string tesselation_control_shader, std::string tesselation_evaluation_shader, std::string geometry_shader_path) {
        bool has_geometry_shader = geometry_shader_path != "";
        bool has_tess_control_shader = tesselation_control_shader != "";
        bool has_tess_evaluation_shader = tesselation_evaluation_shader != "";

        /* Create the shaders */
        int ret;
        GLuint vertex_shader_id = CompileShader(vertex_file_path, GL_VERTEX_SHADER, ret);
        GLuint fragment_shader_id = CompileShader(fragment_file_path, GL_FRAGMENT_SHADER, ret);
        GLuint geometry_shader_id = (has_geometry_shader) ? (CompileShader(geometry_shader_path, GL_GEOMETRY_SHADER, ret)) : 0;
        GLuint tesselation_control_shader_id = (has_tess_control_shader) ? (CompileShader(tesselation_control_shader, GL_TESS_CONTROL_SHADER, ret)) : 0;
        GLuint tesselation_evaluation_shader_id = (has_tess_evaluation_shader) ? (CompileShader(tesselation_evaluation_shader, GL_TESS_EVALUATION_SHADER, ret)) : 0;

        /* Link the program */
        GLint result = GL_FALSE;
        int InfoLogLength;
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, vertex_shader_id);
        glAttachShader(ProgramID, fragment_shader_id);
        if (has_geometry_shader) glAttachShader(ProgramID, geometry_shader_id);
        if (has_tess_control_shader) glAttachShader(ProgramID, tesselation_control_shader_id);
        if (has_tess_evaluation_shader) glAttachShader(ProgramID, tesselation_evaluation_shader_id);
        glLinkProgram(ProgramID);

        /* Check the program */
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0 && result == GL_FALSE) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    
            debug_tools::Console(debug_tools::FATAL, std::string(ProgramErrorMessage.begin(), ProgramErrorMessage.end()));
            return Error::ERROR_SHADER_COMPILE;
        }
    
    
        glDetachShader(ProgramID, vertex_shader_id);
        glDetachShader(ProgramID, fragment_shader_id);
        if (has_geometry_shader) glDetachShader(ProgramID, geometry_shader_id);
        if (has_tess_control_shader) glDetachShader(ProgramID, tesselation_control_shader_id);
        if (has_tess_evaluation_shader) glDetachShader(ProgramID, tesselation_evaluation_shader_id);
    
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        if (has_geometry_shader) glDeleteShader(geometry_shader_id);
        if (has_tess_control_shader) glDeleteShader(tesselation_control_shader_id);
        if (has_tess_evaluation_shader) glDeleteShader(tesselation_evaluation_shader_id);
    
        program_id_ = ProgramID;
        return 0;
    }

    int OpenGLShader::CompileShader(std::string file_path, GLuint type, int& ret)
    {
        GLuint shader_id = glCreateShader(type);

        /* Read the Shader code from the file */
        std::string shader_code;
        std::ifstream stream(file_path, std::ios::in);
        if (stream.is_open()) {
            std::string line = "";
            while (std::getline(stream, line))
                shader_code += "\n" + line;
            stream.close();
        }
        else {
            ret = Error::ERROR_SHADER_FILES_NOT_FOUND;
            debug_tools::Console(debug_tools::FATAL, "Can't open shader file: " + file_path);
            return -1;
        }

        /* Compile Shader */
        GLint result = GL_FALSE;
        int InfoLogLength;
        char const * source_pointer = shader_code.c_str();
        glShaderSource(shader_id, 1, &source_pointer, NULL);
        glCompileShader(shader_id);
        /* Check Vertex Shader */
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0 && result == GL_FALSE) {
            std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shader_id, InfoLogLength, NULL, &ShaderErrorMessage[0]);

            ret = Error::ERROR_SHADER_COMPILE;
            debug_tools::Console(debug_tools::FATAL, std::string(ShaderErrorMessage.begin(), ShaderErrorMessage.end()));
            return -1;
        }

        return shader_id;
    }
    
    bool OpenGLShader::Use() {
        if (!is_inited_) return false;
        glUseProgram(program_id_);
        return true;
    }

    GLint OpenGLShader::GetAttributeLocation(std::string attribute_name) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        GLint ret = glGetAttribLocation(program_id_, attribute_name.c_str());
#ifdef _DEBUG
        if (ret == -1) dt::ConsoleInfoL(dt::CRITICAL, "Shader attribute not found", "name", attribute_name);
#endif
        return ret;
    }

    GLint OpenGLShader::GetUniformLocation(std::string uniform_name) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;
    
        GLint ret = glGetUniformLocation(program_id_, uniform_name.c_str());
#ifdef _DEBUG
        if (ret == -1) dt::ConsoleInfoL(dt::CRITICAL, "Shader uniform not found", "name", uniform_name);
#endif
        return ret;
    }

    void OpenGLShader::SetUniformMat4(GLuint id, glm::mat4& model) {
        glUniformMatrix4fv(id, 1, GL_FALSE, &model[0][0]);
    }
    
    void OpenGLShader::SetUniformVec3(GLuint id, glm::vec3& vector) {
        glUniform3fv(id, 1, &vector[0]);
    }
    
    void OpenGLShader::SetUniformFloat(GLuint id, float value) {
        glUniform1fv(id, 1, &value);
    }
    
    void OpenGLShader::SetUniformInt(GLuint id, int value) {
        glUniform1i(id, value);
    }
    
    void OpenGLShader::SetUniformUInt(GLuint id, unsigned int value) {
        glUniform1ui(id, value);
    }
    
    void OpenGLShader::SetUniformBool(GLuint id, bool value) {
        SetUniformInt(id, static_cast<int>(value));
    }
    
    OpenGLShaderText::OpenGLShaderText() :OpenGLShader() {
    
    }
    
    int OpenGLShaderText::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_ = GetAttributeLocation(shader_text_name_vertex)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Texture_ = GetUniformLocation(shader_text_name_uni_texture)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Texture_color_ = GetUniformLocation(shader_text_name_uni_texture_color)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
    
        return 0;
    }

    OpenGLShaderText3D::OpenGLShaderText3D() {
    }

    int OpenGLShaderText3D::Init(std::string vertex_shader_path, std::string fragment_shader_path) {

        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;

        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_uv_ = GetAttributeLocation(shader_vertex_uv)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Model_ = GetUniformLocation(shader_uni_model)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_View_ = GetUniformLocation(shader_uni_view)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Texture_ = GetUniformLocation(shader_text_name_uni_texture)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Texture_color_ = GetUniformLocation(shader_text_name_uni_texture_color)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }
    
    OpenGLShaderVerticesColor::OpenGLShaderVerticesColor() {
    }
    
    int OpenGLShaderVerticesColor::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Model_ = GetUniformLocation(shader_uni_model)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_View_ = GetUniformLocation(shader_uni_view)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_fragment_color_ = GetUniformLocation(shader_vertices_color_uni_color)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
    
        return 0;
    }
    
    OpenGLShaderGBuffer::OpenGLShaderGBuffer() {
    
    }
    
    int OpenGLShaderGBuffer::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_uv_ = GetAttributeLocation(shader_vertex_uv)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_normal_ = GetAttributeLocation(shader_vertex_normal)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Model_ = GetUniformLocation(shader_uni_model)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_View_ = GetUniformLocation(shader_uni_view)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Lightspace_ = GetUniformLocation(shader_uni_lightspace)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
    
        return 0;
    }

    OpenGLShaderSSAO::OpenGLShaderSSAO() {
    }
    
    int OpenGLShaderSSAO::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_uv_ = GetAttributeLocation(shader_vertex_uv)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_g_position_ = GetUniformLocation(shader_gbuffer_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_g_normal_ = GetUniformLocation(shader_gbuffer_normal)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_noise_texture_ = GetUniformLocation(shader_ssao_noise_texture)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_radius_ = GetUniformLocation(shader_ssao_radius)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_samples_size_ = GetUniformLocation(shader_ssao_samples_size)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_intensity_ = GetUniformLocation(shader_ssao_intensity)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_bias_ = GetUniformLocation(shader_ssao_bias)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_matrix_projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
    
        return 0;
    }
    
    OpenGLShaderFinalPass::OpenGLShaderFinalPass()
    {
    }
    
    int OpenGLShaderFinalPass::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_uv_ = GetAttributeLocation(shader_vertex_uv)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_texture_gbuffer_position_ = GetUniformLocation(shader_gbuffer_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_texture_gbuffer_normal_ = GetUniformLocation(shader_gbuffer_normal)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_texture_gbuffer_albedo_spec_ = GetUniformLocation(shader_gbuffer_albedo_spec)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_texture_gbuffer_position_light_ = GetUniformLocation(shader_gbuffer_position_light)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_texture_ssao_ = GetUniformLocation(shader_final_pass_ssao_texture)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_shadow_map_ = GetUniformLocation(shader_uni_shadow_map)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_matrix_view_ = GetUniformLocation(shader_uni_view)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_matrix_projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }
    
    OpenGLShaderSeparableAO::OpenGLShaderSeparableAO() {
    }
    
    int OpenGLShaderSeparableAO::Init(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_uv_ = GetAttributeLocation(shader_vertex_uv)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_g_position_ = GetUniformLocation(shader_gbuffer_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_g_normal_ = GetUniformLocation(shader_gbuffer_normal)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_noise_texture_ = GetUniformLocation(shader_ssao_noise_texture)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_radius_ = GetUniformLocation(shader_ssao_radius)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_samples_size_ = GetUniformLocation(shader_ssao_samples_size)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_intensity_ = GetUniformLocation(shader_ssao_intensity)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_bias_ = GetUniformLocation(shader_ssao_bias)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_matrix_projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
    
        return 0;
    }
    
    OpenGLShaderShadowMap::OpenGLShaderShadowMap(){
    }
    
    int OpenGLShaderShadowMap::Init(std::string vertex_shader_path, std::string fragment_shader_path){
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path);
        if (ret != 0) return ret;
    
        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Model_ = GetUniformLocation(shader_uni_model)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Lightspace_ = GetUniformLocation(shader_uni_lightspace)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
    
        return 0;
    }

    OpenGLShaderDrawNormals::OpenGLShaderDrawNormals() {

    }

    int OpenGLShaderDrawNormals::Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string geometry_shader_path)
    {
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path, geometry_shader_path);
        if (ret != 0) return ret;

        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_normal_ = GetAttributeLocation(shader_vertex_normal)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Model_ = GetUniformLocation(shader_uni_model)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_View_ = GetUniformLocation(shader_uni_view)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }

    OpenGLShaderWater::OpenGLShaderWater()
    {
    }

    int OpenGLShaderWater::Init(std::string vertex_shader_path, std::string fragment_shader_path, std::string tesselation_control_shader, std::string tesselation_evaluation_shader)
    {
        int ret = OpenGLShader::Init(vertex_shader_path, fragment_shader_path, tesselation_control_shader, tesselation_evaluation_shader);
        if (ret != 0) return ret;

        if ((attr_vertex_position_ = GetAttributeLocation(shader_vertex_position)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_uv_ = GetAttributeLocation(shader_vertex_uv)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((attr_vertex_normal_ = GetAttributeLocation(shader_vertex_normal)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Model_ = GetUniformLocation(shader_uni_model)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_View_ = GetUniformLocation(shader_uni_view)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_Projection_ = GetUniformLocation(shader_uni_projection)) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_camera_world_position_ = GetUniformLocation("camera_world_position")) == -1)return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((uni_displacement_map_ = GetUniformLocation("displacement_map")) == -1) return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }

    
}
}
}

