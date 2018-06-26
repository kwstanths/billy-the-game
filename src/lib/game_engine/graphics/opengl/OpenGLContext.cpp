#include "OpenGLContext.hpp"

#include "game_engine/ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace game_engine {
namespace graphics {
namespace opengl {

    OpenGLContext::OpenGLContext() {
        is_inited_ = false;
    }

    int OpenGLContext::Init(OpenGLContextConfig_t config) {
        if (is_inited_) return -1;
     
        config_ = config;

        GLfloat window_ratio_ = (config_.window_width_ * 1.0f) / (config_.window_height_ * 1.0f);

        /* Initialise GLFW */
        if (!glfwInit()) {
            return Error::ERROR_GLFW_INIT;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                /* To make MacOS happy; should not be needed */
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      /* We don't want old OpenGL */

                                                                            /* Open a window and create its OpenGL context */
        glfw_window_ = glfwCreateWindow(config_.window_width_, config_.window_height_, config_.window_name_.c_str(), NULL, NULL);
        if (glfw_window_ == NULL) {
            glfwTerminate();
            return Error::ERROR_GLFW_WINDOW;
        }

        /* Initialize GLEW */
        glfwMakeContextCurrent(glfw_window_);
        glewExperimental = true; /* Needed for core profile */
        if (glewInit() != GLEW_OK) {
            glfwTerminate();
            return Error::ERROR_GLEW_INIT;
        }

        glViewport(0, 0, config_.window_width_, config_.window_height_);
        glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        int ret;
        ret = shader_main_.Init("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
        if (ret != 0) return ret;
        ret = shader_text_.Init("shaders/TextVertexShader.glsl", "shaders/TextFragmentShader.glsl");
        if (ret != 0) return ret;
        ret = shader_simple_.Init("shaders/VertexShaderSimple.glsl", "shaders/FragmentShaderSimple.glsl");
        if (ret != 0) return ret;

        is_inited_ = true;
        return 0;
    }

    size_t OpenGLContext::GetWindowWidth() {
        if (!is_inited_) return 0;

        return config_.window_width_;
    }

    size_t OpenGLContext::GetWindowHeight() {
        if (!is_inited_) return false;

        return config_.window_height_;
    }

    int OpenGLContext::Destroy() {
        if (!is_inited_) return -1;


        glfwTerminate();
        glfw_window_ = NULL;

        is_inited_ = false;
        return 0;
    }

    KeyControls_t OpenGLContext::GetControlsInput() {
        if (!is_inited_) return KeyControls_t();

        KeyControls_t key_controls;

        if (glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) key_controls.KEY_ESC = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_ENTER) == GLFW_PRESS) key_controls.KEY_ENTER = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) key_controls.KEY_LSHIFT = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_A) == GLFW_PRESS) key_controls.KEY_A = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_S) == GLFW_PRESS) key_controls.KEY_S = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_D) == GLFW_PRESS) key_controls.KEY_D = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_W) == GLFW_PRESS) key_controls.KEY_W = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_E) == GLFW_PRESS) key_controls.KEY_E = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_UP) == GLFW_PRESS) key_controls.KEY_ARROW_UP = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_DOWN) == GLFW_PRESS) key_controls.KEY_ARROW_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT) == GLFW_PRESS) key_controls.KEY_ARROW_LEFT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_RIGHT) == GLFW_PRESS) key_controls.KEY_ARROW_RIGHT = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) key_controls.KEY_PAGE_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_PAGE_UP) == GLFW_PRESS) key_controls.KEY_PAGE_UP = true;

        key_controls.timestamp_ = glfwGetTime();

        return key_controls;
    }

    OpenGLShaderMain OpenGLContext::GetShaderMain() {
        if (!is_inited_) return OpenGLShaderMain();

        return shader_main_;
    }

    OpenGLShaderSimple OpenGLContext::GetShaderSimple() {
        if (!is_inited_) return OpenGLShaderSimple();

        return shader_simple_;
    }

    OpenGLShaderText OpenGLContext::GetShaderText() {
        if (!is_inited_) return OpenGLShaderText();

        return shader_text_;
    }

    std::string OpenGLContext::GetFontLocation() {
        return config_.font_file_path;
    }

    int OpenGLContext::ClearColor() {
        if (!is_inited_) return -1;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        return 0;
    }

    int OpenGLContext::SwapBuffers() {
        if (!is_inited_) return -1;

        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();
        return 0;
    }


}
}
}