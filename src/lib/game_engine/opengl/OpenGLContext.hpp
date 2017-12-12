#ifndef __OpenGLContext_hpp__
#define __OpenGLContext_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

#include "OpenGLShaderVariables.hpp"
#include "../ControlInput.hpp"

namespace game_engine {

    class OpenGLContext {
    public:
        OpenGLContext();

        /**
            Initializes the window GLFW and GLEW,
            @param opengl_version_major The minimum OpenGL major version required
            @param opengl_version_minor THe minimum OpenGL minor version required
            @param width The width of the window to create in pixels
            @param height The height of the window to create in pixels
            @param The name of the window to crate
            @return 0=OK, -1=Already initialized, anything else from ErrorCodes
        */
        int Init(size_t opengl_version_major,
            size_t opengl_version_minor,
            size_t width,
            size_t height,
            std::string name
        );

        /**
            Terminates and deallocates window
            @return 0=OK, -1=Not initialized
        */
        int Destroy();

        /**
            Get a struct with input from the user. If init hasn't been called returns all false struct
            @return A ControlInput_t struct
        */
        ControlInput_t GetControlsInput();

        /**
            Clears the color of the screen
            @return 0=OK, -1=Not initialsed
        */
        int ClearColor();

        /**
            Swap window buffers
            @return 0=OK, -1=Not initialsed
        */
        int SwapBuffers();

    private:
        bool is_inited_;
        size_t window_width_;
        size_t window_height_;
        GLfloat window_ratio_;
        GLFWwindow * glfw_window_ = nullptr;
        OpenGLShaderVariables_t shader_vars_;

        /**
        
        */
        int LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path);
    };

}

#endif
