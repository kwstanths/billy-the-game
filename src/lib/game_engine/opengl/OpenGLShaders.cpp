#include "OpenGLShaders.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>

#include "../ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

namespace game_engine {

    int OpenGLLoadShaders(std::string vertex_shader_path, std::string fragment_shader_path, OpenGLShaderVariables_t * shader_vars) {
        
        int ret = OpenGLCompileShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str(), &(shader_vars->program_id_));
        if (ret != 0) return ret;

        if ((shader_vars->attr_vertex_position_ = glGetAttribLocation(shader_vars->program_id_, shader_name_vertex_position)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->attr_vertex_uv_ = glGetAttribLocation(shader_vars->program_id_, shader_name_vertex_uv)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_Model_ = glGetUniformLocation(shader_vars->program_id_, shader_name_uni_model)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_View_ = glGetUniformLocation(shader_vars->program_id_, shader_name_uni_view)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_Projection_ = glGetUniformLocation(shader_vars->program_id_, shader_name_uni_projection)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_Texture_ = glGetUniformLocation(shader_vars->program_id_, shader_name_uni_texture)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }

    int OpenGLLoadShaders(std::string vertex_shader_path, std::string fragment_shader_path, OpenGLShaderTextVariables_t * shader_vars) {

        int ret = OpenGLCompileShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str(), &(shader_vars->program_id_));
        if (ret != 0) return ret;

        if ((shader_vars->attr_vertex_ = glGetAttribLocation(shader_vars->program_id_, shader_text_name_vertex)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_Projection_ = glGetUniformLocation(shader_vars->program_id_, shader_text_name_uni_projection)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_Texture_ = glGetUniformLocation(shader_vars->program_id_, shader_text_name_uni_texture)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars->uni_Texture_color_ = glGetUniformLocation(shader_vars->program_id_, shader_text_name_uni_texture_color)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }

    int OpenGLCompileShaders(const char * vertex_file_path, const char * fragment_file_path, GLuint * program_id) {
        /* Create the shaders */
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        /* Read the Vertex Shader code from the file */
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if (VertexShaderStream.is_open()) {
            std::string Line = "";
            while (getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }
        else
            return Error::ERROR_SHADER_FILES_NOT_FOUND;


        /* Read the Fragment Shader code from the file */
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open()) {
            std::string Line = "";
            while (getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        }
        else
            return Error::ERROR_SHADER_FILES_NOT_FOUND;

        GLint Result = GL_FALSE;
        int InfoLogLength;
        /* Compile Vertex Shader */
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);
        /* Check Vertex Shader */
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0 && Result == GL_FALSE) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);

            debug_tools::Console(debug_tools::FATAL, std::string(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end()));
            return Error::ERROR_SHADER_COMPILE;
        }

        /* Compile Fragment Shader */
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);
        /* Check Fragment Shader */
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0 && Result == GL_FALSE) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);

            debug_tools::Console(debug_tools::FATAL, std::string(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end()));
            return Error::ERROR_SHADER_COMPILE;
        }

        /* Link the program */
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);
        /* Check the program */
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0 && Result == GL_FALSE) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);

            debug_tools::Console(debug_tools::FATAL, std::string(ProgramErrorMessage.begin(), ProgramErrorMessage.end()));
            return Error::ERROR_SHADER_COMPILE;
        }


        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        *program_id = ProgramID;
        return 0;
    }

}

