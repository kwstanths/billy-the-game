#include "GameEngine.hpp"

#include "ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameEngine::GameEngine() {
        is_inited_ = false;

        context_ = new OpenGLContext();
        camera_ = new OpenGLCamera();
        test_object_1_ = new OpenGLObject();
    }

    GameEngine::~GameEngine() {
        delete camera_;
        delete context_;
    }

    int GameEngine::Init(size_t opengl_version_major, size_t opengl_version_minor, size_t width, size_t height, std::string name) {
        if (is_inited_) return -1;

        int ret = context_->Init(opengl_version_major, opengl_version_minor, width, height, name);
        if (ret != 0) {
            dt::Console(dt::FATAL, "Error: " + std::to_string(ret));
            Terminate();
        }

        camera_->Init(0, 0, 0, 
            0, 0, 1, 
            0, 1, 0, 
            context_);
        
        test_object_1_->Init("", "", 0, 0, 10, context_);

        CodeReminder("Support key remapping");

        is_inited_ = true;
        return 0;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) return -1;

        test_object_1_->Destroy();
        camera_->Destroy();
        context_->Destroy();

        is_inited_ = false;
        return 0;
    }

    void GameEngine::Step(double delta_time) {
        context_->ClearColor();

        camera_->SetView();
        test_object_1_->Draw(delta_time);

        context_->SwapBuffers();
    }

    ControlInput_t GameEngine::GetControlsInput() {
        return context_->GetControlsInput();
    }

    void GameEngine::Fun(float test_x, float test_y, float test_z) {
        camera_->Fun(test_x, test_y, test_z);
    }

    void GameEngine::Terminate() {
        dt::WaitInput();
        exit(-1);
    }

}
