#include "OpenGLContext.hpp"

#include "common/OpenGLLoadShaders.hpp"

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

        glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        int ret = LoadShaders(config_.shader_vertex_file_path_.c_str(), config_.shader_fragment_file_path_.c_str());
        if (ret != 0) return ret;

        glUseProgram(shader_vars_.program_id_);

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

    int OpenGLContext::LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path) {
    
        int ret = OpenGLLoadShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str(), &(shader_vars_.program_id_));
        if (ret != 0) return ret;

        if ((shader_vars_.attr_vertex_position_ = glGetAttribLocation(shader_vars_.program_id_, shader_name_vertex_position)) == -1) 
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars_.attr_vertex_uv_ = glGetAttribLocation(shader_vars_.program_id_, shader_name_vertex_uv)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars_.uni_Model_ = glGetUniformLocation(shader_vars_.program_id_, shader_name_uni_model)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars_.uni_View_ = glGetUniformLocation(shader_vars_.program_id_, shader_name_uni_view)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars_.uni_Projection_ = glGetUniformLocation(shader_vars_.program_id_, shader_name_uni_projection)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;
        if ((shader_vars_.uni_Texture_ = glGetUniformLocation(shader_vars_.program_id_, shader_name_uni_texture)) == -1)
            return Error::ERROR_SHADER_RES_NOT_FOUND;

        return 0;
    }

}
