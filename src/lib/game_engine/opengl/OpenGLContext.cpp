#include "OpenGLContext.hpp"

#include "../ErrorCodes.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace game_engine {

    OpenGLContext::OpenGLContext(OpenGLContextConfig_t config) {
        is_inited_ = false;

        config_ = config;
    }

    int OpenGLContext::Init() {
        if (is_inited_) return -1;
        
        GLfloat window_ratio_ = (config_.window_width_ * 1.0f) / (config_.window_height_ * 1.0f);

        /* Initialise GLFW */
        if (!glfwInit()) {
            return Error::ERROR_GLFW_INIT;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config_.opengl_version_major_);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config_.opengl_version_minor_);
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
        ret = OpenGLLoadShaders(config_.shader_vertex_file_path_, config_.shader_fragment_file_path_, &shader_vars_);
        if (ret != 0) return ret;
        ret = OpenGLLoadShaders(config_.shader_vertex_text_file_path, config_.shader_fragment_text_file_path, &shader_text_vars_);
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

    ControlInput_t OpenGLContext::GetControlsInput() {
        if (!is_inited_) return ControlInput_t();

        ControlInput_t input;

        if (glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) input.KEY_ESC = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_E) == GLFW_PRESS) input.KEY_ACT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_ENTER) == GLFW_PRESS) input.KEY_ENTER = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) input.KEY_RUN = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_UP) == GLFW_PRESS) input.KEY_UP = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_W) == GLFW_PRESS) input.KEY_UP = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_DOWN) == GLFW_PRESS) input.KEY_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_S) == GLFW_PRESS) input.KEY_DOWN = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT) == GLFW_PRESS) input.KEY_LEFT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_A) == GLFW_PRESS) input.KEY_LEFT = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_RIGHT) == GLFW_PRESS) input.KEY_RIGHT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_D) == GLFW_PRESS) input.KEY_RIGHT = true;

        if (glfwGetKey(glfw_window_, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) input.KEY_PAGE_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_PAGE_UP) == GLFW_PRESS) input.KEY_PAGE_UP = true;

        return input;
    }

    OpenGLShaderVariables_t OpenGLContext::GetShaderVariables() {
        if (!is_inited_) return OpenGLShaderVariables_t();

        return shader_vars_;
    }

    OpenGLShaderTextVariables_t OpenGLContext::GetShaderTextVariables() {
        if (!is_inited_) return OpenGLShaderTextVariables_t();

        return shader_text_vars_;
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
