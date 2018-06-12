#include "GameEngine.hpp"

#include "ErrorCodes.hpp"
#include "physics/Types.hpp"

#include "WorldSector.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameEngine::GameEngine() {
        is_inited_ = false;
        last_error_ = 0;

        context_ = new OpenGLContext();
        camera_ = new OpenGLCamera();
        renderer_ = new OpenGLRenderer();
        asset_manager_ = new AssetManager();
        debugger_ = new Debugger();
    }

    GameEngine::~GameEngine() {
        if (renderer_ != nullptr) delete renderer_;
        if (camera_ != nullptr) delete camera_;
        if (context_ != nullptr) delete context_;
        if (asset_manager_ != nullptr) delete asset_manager_;
        if (debugger_!= nullptr) delete debugger_;

        renderer_ = nullptr;
        camera_ = nullptr;
        context_ = nullptr;
        asset_manager_ = nullptr;
        debugger_ = nullptr;
    }

    int GameEngine::Init(GameEngineConfig_t config) {
        if (is_inited_) {
            last_error_ = -1;
            return -1;
        }

        config_ = config;
        
        int ret = context_->Init(config.context_params_);
        if (ret != 0) {
            last_error_ = ret;
            PrintError(ret);
            Terminate();
        }

        camera_->Init(config.camera_params_, context_);
        renderer_->Init(context_);
        asset_manager_->Init(200, 200);
        debugger_->Init(asset_manager_, renderer_);
        frame_regulator_.Init(config_.frame_rate_, 5);

        CodeReminder("Find the size and margin of the visible world based on the camera");
        visible_world_ = std::vector<WorldObject *>(200);

        CodeReminder("Support key remapping");

        /* Init random numbers generator */
        srand(time(NULL));       

        is_inited_ = true;
        last_error_ = 0;
        return last_error_;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) {
            last_error_ = -1;
            return last_error_;
        }

        camera_->Destroy();
        context_->Destroy();
        /* TODO Implement renderer Destroy() which depends on OpenGLContext */
        asset_manager_->Destroy();
        debugger_->Destroy();
        frame_regulator_.Destroy();

        is_inited_ = false;
        last_error_ = 0;
        return last_error_;
    }

    bool GameEngine::IsInited() {
        return is_inited_;
    }

    void GameEngine::Step(double delta_time) {

        /* Start the frame */
        frame_regulator_.FrameStart();

        /* Get latest control values */
        key_controls_ = context_->GetControlsInput();

        context_->ClearColor();

        /* Get visible items */
        float center_x, center_y, center_z;
        camera_->GetPosition(&center_x, &center_y, &center_z);

        /* TODO Check whether sector_ is set */
        /* TODO Find the visible items based on the z of the camera */
        size_t nof = sector_->GetObjectsWindow(center_x, center_y, 4, visible_world_);

        /* Step all the objects one frame */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Step(delta_time);
        }

        /* 
            Set camera's view before drawing, because Step() might have tempered with the camera position
        */
        camera_->SetView();

        /* Draw visible world */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Draw(renderer_);
        }

        /* Render text overlay */
        renderer_->Draw2DText("Welcome!", 100, 100, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
        
        /* Update world */
        sector_->DeleteRemovedObjects();

        context_->SwapBuffers();

        /* End the frame */
        frame_regulator_.FrameEnd();
    }

    KeyControls_t GameEngine::GetControlsInput() {
        return key_controls_;
    }

    float GameEngine::GetFrameDelta() {
        return frame_regulator_.GetDelta();
    }

    void GameEngine::CameraMove2D(float move_x, float move_y) {
        camera_->Move(move_x, move_y, 0);
    }

    void GameEngine::CameraZoom2D(float factor) {
        camera_->Zoom(factor);
    }

    int GameEngine::SetWorld(WorldSector * world) {
        if (world == nullptr) {
            last_error_ = Error::ERROR_WORLD_NOT_INIT;
            PrintError(last_error_);
            return last_error_;
        }

        if (!world->IsInited()) {
            last_error_ = Error::ERROR_WORLD_NOT_INIT;
            PrintError(last_error_);
            return last_error_;
        }
        sector_ = world;
        return 0;
    }

    AssetManager * GameEngine::GetAssetManager() {
        return asset_manager_;
    }

    Debugger * GameEngine::GetDebugger() {
        return debugger_;
    }

    int GameEngine::GetLastError() {
        return last_error_;
    }

    void GameEngine::Terminate() {
        dt::WaitInput();
        exit(-1);
    }

}
