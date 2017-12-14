#include "OpenGLLoadShaders.hpp"

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

    int OpenGLLoadShaders(const char * vertex_file_path, const char * fragment_file_path, GLuint * program_id) {

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
        } else
            return Error::ERROR_SHADER_FILES_NOT_FOUND;


        /* Read the Fragment Shader code from the file */
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if (FragmentShaderStream.is_open()) {
            std::string Line = "";
            while (getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        } else
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

