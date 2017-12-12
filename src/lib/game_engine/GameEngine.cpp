#include "GameEngine.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameEngine::GameEngine() {
        is_inited_ = false;

        context_ = new OpenGLContext();
        camera_ = new Camera();
    }

    int GameEngine::Init(size_t opengl_version_major, size_t opengl_version_minor, size_t width, size_t height, std::string name) {
        if (is_inited_) return -1;

        int ret = context_->Init(opengl_version_major, opengl_version_minor, width, height, name);
        if (ret != 0) {
            dt::Console(dt::FATAL, "Error: " + std::to_string(ret));
            return ret;
        }



        camera_->Init(0, 0, -10, 0, 0, 0, 0, 1, 0);

        CodeReminder("Support key remapping");

        is_inited_ = true;
        return 0;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) return -1;

        context_->Destroy();

        is_inited_ = false;
        return 0;
    }

    void GameEngine::Step() {
        context_->ClearColor();



        context_->SwapBuffers();
    }

    ControlInput_t GameEngine::GetControlsInput() {
        return context_->GetControlsInput();
    }

}
