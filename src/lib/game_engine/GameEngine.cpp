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
        if (renderer_ != nullptr) delete renderer_;
        if (camera_ != nullptr) delete camera_;
        if (context_ != nullptr) delete context_;
        
        renderer_ = nullptr;
        camera_ = nullptr;
        context_ = nullptr;
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

        CodeReminder("Find the size of the visible world based on the camera");
        CodeReminder("Find the margin of the visible world based on the camera");
        visible_world_ = std::vector<WorldObject *>(200);

        CodeReminder("Support key remapping");

        is_inited_ = true;
        return 0;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) return -1;

        camera_->Destroy();
        context_->Destroy();

        is_inited_ = false;
        return 0;
    }

    void GameEngine::Step(double delta_time) {
        context_->ClearColor();

        /* Get visible items */
        float center_x, center_y, center_z;
        camera_->GetPosition(&center_x, &center_y, &center_z);

        size_t nof = sector_->GetObjectsWindow(center_x, center_y, 10, visible_world_);

        /* Set camera's view */
        camera_->SetView();

        /* Draw visible world */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Step(delta_time);
            visible_world_[i]->Draw();
        }

        /* Draw main actors */
        main_actor_->Step(delta_time);
        main_actor_->Draw();

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

    int GameEngine::WorldObjectInit(WorldObject * obj, OpenGLObject * object, OpenGLTexture * texture) {
        if (!is_inited_) return -1;
        if (obj == nullptr || object == nullptr || texture == nullptr) {
            return -1;
        }

        int ret = obj->Init(object, texture, renderer_);

        return ret;
    }

    int GameEngine::AddWorldSector(WorldSector * sector) {
        sector_ = sector;
        return 0;
    }

    int GameEngine::AddMainActor(WorldObject * object) {
        main_actor_ = object;
        return 0;
    }

    void GameEngine::Terminate() {
        dt::WaitInput();
        exit(-1);
    }

}
