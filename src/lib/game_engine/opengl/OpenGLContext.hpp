#ifndef __OpenGLContext_hpp__
#define __OpenGLContext_hpp__

#include <string>

#include "OpenGLIncludes.hpp"

#include "OpenGLShaders.hpp"
#include "../ControlInput.hpp"

namespace game_engine {

    /**
        Values necessary to initialize the OpenGLContext object
    */
    typedef struct {
        /* The maximum OpenGL major version required */
        size_t opengl_version_major_;
        /* The minimum OpenGL minor version required */
        size_t opengl_version_minor_;
        /* The width of the window to create in pixels */
        size_t window_width_;
        /* The height of the window to create in pixels */
        size_t window_height_;
        /* The name of the window to create */
        std::string window_name_;
        /* The vertex shader path in the disk */
        std::string shader_vertex_file_path_;
        /* The fragment shader path in the disk */
        std::string shader_fragment_file_path_;
    } OpenGLContextConfig_t;


    class OpenGLContext {
    public:
        /**
            Sets the parameters to the local variable, does nothing else. Feel free to discard  the params object 
            after this call
            @param param The necessary stuff when Init() is called
        */
        OpenGLContext(OpenGLContextConfig_t params);

        /**
            Uses the parameters set to initialise and OpenGL context. Initializes  the window, GLFW, GLEW, reads 
            the shaders. After this call feel free to call
            GetShaderVariables to get the shaders IDs
            @return 0=OK, -1=Already initialized, anything else see ErrorCodes
        */
        int Init();

        /**
            Get the window width set
            @return 0=Not initialised
        */
        size_t GetWindowWidth();

        /**
            Get the window height set
            @return 0=Not initialised
        */
        size_t GetWindowHeight();

        /**
            Terminates GLFW and deallocates the window
            @return 0=OK, -1=Not initialized
        */
        int Destroy();

        /**
            Get a struct with input from the user. If init hasn't been called returns an all false  struct
            @return A ControlInput_t struct
        */
        ControlInput_t GetControlsInput();

        /**
            Get shader variables
        */
        OpenGLShaderVariables_t GetShaderVariables();

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
        OpenGLContextConfig_t config_;
        OpenGLShaderVariables_t shader_vars_;

        GLFWwindow * glfw_window_ = nullptr;

        /**
            Loads and compiles the shaders from the disk
        */
        int LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path);
    };

}

#endif
