#ifndef __OpenGLContext_hpp__
#define __OpenGLContext_hpp__

#include <string>

#include "game_engine/Controls.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLShaders.hpp"


#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

namespace game_engine {
    namespace graphics {
        namespace opengl {

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
            } OpenGLContextConfig_t;


            class OpenGLContext {
                friend class OpenGLCamera;
                friend class OpenGLRenderer;
                friend class OpenGLText;
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
                    Check whether or not the object is initialised
                    @param true = OK, false = NOT OK
                */
                bool IsInited();

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


                void SetWindowSize(size_t width, size_t height);

                /**
                    Get a struct with input from the user. If init hasn't been called returns an all false  struct
                    @return A ControlInput_t struct
                */
                KeyControls_t GetControlsInput();

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

                OpenGLShaderVerticesColor shader_vertices_color_;
                OpenGLShaderText shader_text_;
                OpenGLShader shader_quad_;
                OpenGLShaderGBuffer shader_gbuffer_ssao_;
                OpenGLShaderSSAO shader_ssao_;
                OpenGLShaderSeparableAO shader_separable_ao_;
                OpenGLShader shader_blur_;
                OpenGLShaderFinalPass shader_final_pass_;
                OpenGLShaderShadowMap shader_shadow_map_;
                OpenGLShaderWater shader_water_;

                GLFWwindow * glfw_window_ = nullptr;
            };

        }
    }
}

#endif
