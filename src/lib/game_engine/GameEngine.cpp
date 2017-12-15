#include "GameEngine.hpp"

#include "ErrorCodes.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameEngine::GameEngine(OpenGLContextConfig_t & context_params, OpenGLCameraConfig_t & camera_params) {
        is_inited_ = false;
        
        CodeReminder("Game engine FPS");

        context_ = new OpenGLContext(context_params);
        camera_ = new OpenGLCamera(camera_params);
    }

    GameEngine::~GameEngine() {
        delete camera_;
        delete context_;
    }

    int GameEngine::Init() {
        if (is_inited_) return -1;
        
        int ret = context_->Init();
        if (ret != 0) {
            dt::Console(dt::FATAL, "Error: " + std::to_string(ret));
            Terminate();
        }

        camera_->Init(context_);
        

        CodeReminder("Support key remapping");

        is_inited_ = true;
        return 0;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) return -1;

        for(size_t i=0; i<objects_.size(); i++) objects_[i]->Destroy();
        camera_->Destroy();
        context_->Destroy();

        is_inited_ = false;
        return 0;
    }

    void GameEngine::Step(double delta_time) {
        context_->ClearColor();

        camera_->SetView();
        for (size_t i = 0; i < objects_.size(); i++) {
            objects_[i]->Step(delta_time);
            objects_[i]->Draw(delta_time);
        }

        context_->SwapBuffers();
    }

    ControlInput_t GameEngine::GetControlsInput() {
        return context_->GetControlsInput();
    }

    void GameEngine::CameraMove2D(float move_x, float move_y) {
        camera_->Move(move_x, move_y, 0);
    }

    void GameEngine::CameraZoom2D(float factor) {
        camera_->Zoom(factor);
    }

    int GameEngine::AddObject(WorldObject * obj, OpenGLObjectConfig_t config) {
        if (!is_inited_) return -1;
        
        int ret = obj->Init(config, context_);
        if (ret != 0) {
            dt::ConsoleInfoL(dt::WARNING, "Cannot load assets",
                "Object", config.object_path_,
                "Texture", config.texture_path_);
            return ret;
        }

        objects_.push_back(obj);

        return 0;
    }

    void GameEngine::Terminate() {
        dt::WaitInput();
        exit(-1);
    }

}
