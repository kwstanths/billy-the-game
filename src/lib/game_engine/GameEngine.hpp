#ifndef __GameEngine_hpp__
#define __GameEngine_hpp__

#ifdef _WIN32
#include <Windows.h>
#elif

#endif

#include <string>

#include <GL/glew.h>
#include <glfw3.h>

#include "FrameRateRegulator.hpp"
#include "ControlInput.hpp"
#include "ErrorCodes.hpp"

namespace game_engine {

    class GameEngine {
    public:
        /**
            Does nothing important
        */
        GameEngine();

        /**
            Initializes the window GLFW and GLEW,
            @param opengl_version_major The minimum OpenGL major version required
            @param opengl_version_minor THe minimum OpenGL minor version required
            @param width The width of the window to create in pixels
            @param height The height of the window to create in pixels
            @param The name of the window to crate
            @return 0=OK, -1=Not initialzed, anything else from ErrorCodes
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
            A single engine step, Should be called inside the main loop
        */
        void Step();

        /**
        
        */
        ControlInput_t GetControlsInput();

    private:
        bool is_inited_;
        size_t window_width_;
        size_t window_height_;
        GLfloat window_ratio_;
        GLFWwindow * glfw_window_;
    };

}

#endif