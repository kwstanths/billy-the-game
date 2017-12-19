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
        renderer_ = new OpenGLRenderer();
    }

    GameEngine::~GameEngine() {
        delete camera_;
        delete context_;
    }

    int GameEngine::Init() {
        if (is_inited_) return -1;
        
        int ret = context_->Init();
        if (ret != 0) {
            PrintError(ret);
            Terminate();
        }

        camera_->Init(context_);
        
        renderer_->Init(context_);

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
            objects_[i]->Draw();
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

    int GameEngine::AddObject(WorldObject * obj, OpenGLObject * object, OpenGLTexture * texture) {
        if (!is_inited_) return -1;
        
        int ret = obj->Init(object, texture, renderer_);

        objects_.push_back(obj);

        return 0;
    }

    void GameEngine::Terminate() {
        dt::WaitInput();
        exit(-1);
    }

}
