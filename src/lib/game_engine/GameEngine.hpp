#ifndef __GameEngine_hpp__
#define __GameEngine_hpp__

#include <string>

#include "opengl/OpenGLContext.hpp"
#include "ControlInput.hpp"
#include "Camera.hpp"

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
            Get a struct with input from the user
            @return A ControlInput_t struct
        */
        ControlInput_t GetControlsInput();

    private:
        bool is_inited_;
        OpenGLContext * context_ = nullptr;
        Camera * camera_ = nullptr;

    };

}

#endif