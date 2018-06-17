#ifndef __OpenGLContext_hpp__
#define __OpenGLContext_hpp__

#include <string>

#include "../Controls.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLShaders.hpp"


#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

namespace game_engine {

    /**
        Values necessary to initialize the OpenGLContext object
    */
    typedef struct {
        /* The width of the window to create in pixels */
        size_t window_width_;
        /* The height of the window to create in pixels */
        size_t window_height_;
        /* The name of the window to create */
        std::string window_name_;
        /* The file path of the font to be used */
        std::string font_file_path;
        /* The vertex shader path in the disk */
        std::string shader_vertex_file_path_;
        /* The fragment shader path in the disk */
        std::string shader_fragment_file_path_;
        /* The text vertex shader */
        std::string shader_vertex_text_file_path;
        /* The text fragment shader */
        std::string shader_fragment_text_file_path;
    } OpenGLContextConfig_t;


    class OpenGLContext {
        friend class OpenGLCamera;
    public:
        /**
            Sets the parameters to the local variable, does nothing else. Feel free to discard  the params object 
            after this call
            @param param The necessary stuff when Init() is called
        */
        OpenGLContext();

        /**
            Uses the parameters set to initialise and OpenGL context. Initializes  the window, GLFW, GLEW, reads 
            the shaders. After this call feel free to call
            GetShaderVariables to get the shaders IDs
            @return 0=OK, -1=Already initialized, anything else see ErrorCodes
        */
        int Init(OpenGLContextConfig_t config);

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
        KeyControls_t GetControlsInput();

        /**
            Get main shader variables
        */
        OpenGLShaderMain GetShaderVariables();

        /**
            Get shader variables for 2d text
        */
        OpenGLShaderText GetShaderTextVariables();

        /**
            Get the font file location
        */
        std::string GetFontLocation();

        /**
            Clears the color of the screen, clear the z-depth buffer
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

        OpenGLShaderMain shader_main_;
        OpenGLShaderText shader_text_;
        /*OpenGLShaderVariables_t shader_vars_;
        OpenGLShaderTextVariables_t shader_text_vars_;
*/
        GLFWwindow * glfw_window_ = nullptr;
    };

}

#endif
