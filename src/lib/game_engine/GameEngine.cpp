#include "GameEngine.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameEngine::GameEngine() {
        is_inited_ = false;
    }

    int GameEngine::Init(size_t opengl_version_major, size_t opengl_version_minor, size_t width, size_t height, std::string name) {
        if (is_inited_) return -1;

        window_width_ = width;
        window_height_ = height;
        window_ratio_ = (width * 1.0f) / (height * 1.0f);

        /* Initialise GLFW */
        if (!glfwInit()) {
            dt::Console(dt::FATAL, "Unable to initialize GLFW");
            return Error::ERROR_GLFW_INIT;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                /* To make MacOS happy; should not be needed */
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      /* We don't want old OpenGL */

        /* Open a window and create its OpenGL context */
        glfw_window_ = glfwCreateWindow(window_width_, window_height_, name.c_str(), NULL, NULL);
        if (glfw_window_ == NULL) {
            dt::ConsoleInfoL(dt::FATAL, "Unable to open GLFW window. OpenGL support is required",
                "OpenGL major requested", opengl_version_major,
                "OpenGL minor requested", opengl_version_minor);
            glfwTerminate();
            return Error::ERROR_GLFW_WINDOW;
        }

        /* Initialize GLEW */
        glfwMakeContextCurrent(glfw_window_);
        glewExperimental = true; /* Needed for core profile */
        if (glewInit() != GLEW_OK) {
            dt::Console(dt::FATAL, "Failed to initialize GLEW");
            glfwTerminate();
            return Error::ERROR_GLFW_WINDOW;
        }

        glfwSetInputMode(glfw_window_, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        CodeReminder("Support key remapping");

        is_inited_ = true;
        return 0;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) return -1;

        glfwTerminate();
        glfw_window_ = NULL;

        is_inited_ = false;
        return 0;
    }

    void GameEngine::Step() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        /* Swap buffers */
        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();
    }

    ControlInput_t GameEngine::GetControlsInput() {
        ControlInput_t input;

        if (glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) input.KEY_ESC = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_E) == GLFW_PRESS) input.KEY_ACT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_ENTER) == GLFW_PRESS) input.KEY_ENTER = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_UP) == GLFW_PRESS) input.KEY_UP = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_DOWN) == GLFW_PRESS) input.KEY_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT) == GLFW_PRESS) input.KEY_LEFT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_RIGHT) == GLFW_PRESS) input.KEY_RIGHT = true;
        
        return input;
    }

}
