#include "GameEngine.hpp"

#include "ErrorCodes.hpp"
#include "game_engine/memory/MemoryManager.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"
namespace dt = debug_tools;
namespace gl = game_engine::graphics::opengl;
namespace grph = game_engine::graphics;

namespace game_engine {

    GameEngine::GameEngine() {
        is_inited_ = false;
        last_error_ = 0;

        renderer_ = new grph::Renderer();
        debugger_ = new Debugger();
    }

    GameEngine::~GameEngine() {
        if (renderer_ != nullptr) delete renderer_;
        if (debugger_!= nullptr) delete debugger_;

        renderer_ = nullptr;
        debugger_ = nullptr;
    }

    int GameEngine::Init(GameEngineConfig_t config) {
        if (is_inited_) {
            last_error_ = -1;
            return -1;
        }

        config_ = config;
        int ret = renderer_->Init(config_.context_params_);
        if (ret) {
            last_error_ = ret;
            PrintError(ret);
            Terminate();
        }

        frame_regulator_.Init(config_.frame_rate_, 10);
        debugger_->Init(renderer_);

        /* Create the one and only MemoryManager object */
        memory::STATIC_OBJETCS_MEMORY_SIZE = 500 * 500;
        memory::REMOVABLE_OBJECTS_MEMORY_BLOCK_SIZE = 400;
        memory::REMOVABLE_OBJECTS_MEMORY_BLOCKS_NUMBER = 1000;
        memory::PHYSICS_OBJECTS_MEMORY_BLOCKS_NUMBER = 20000;
        memory::LIGHT_OBJECTS_MEMORY_BLOCKS_NUMBER = 10000;
        memory::MemoryManager& just_create_the_object_instance = memory::MemoryManager::GetInstance();

        /* Initialize standard library random numbers */
        srand(static_cast<unsigned int>(time(NULL)));

        is_inited_ = true;
        last_error_ = 0;
        return last_error_;
    }

    int GameEngine::Destroy() {
        if (!is_inited_) {
            last_error_ = -1;
            return last_error_;
        }

        renderer_->Destroy();
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

        /* Start calculating frame time */
        frame_regulator_.FrameStart();

        /* Get latest control values */
        key_controls_ = renderer_->GetControlInput();

        renderer_->StartFrame();

        /* Calculate visible rectangle upon the z=0 pane */
        Real_t center_x, center_y, center_z;
        camera_->GetPositionVector(&center_x, &center_y, &center_z);
        Real_t ratio = (Real_t) config_.context_params_.window_width_ / (Real_t) config_.context_params_.window_height_;
        Real_t angle = camera_->GetPerspectiveAngle();
        Real_t width = center_z * tan(angle/2.0f);
        /* 2 * width whould be exactly inside the camera view, 4* gives us a little bigger rectangle */
        math::Rectangle2D camera_view_rectangle = math::Rectangle2D(center_x, center_y, 4 * width * ratio, 4 * width);

        /* Perform one step on the active sector */
        sector_->Step(camera_view_rectangle, delta_time, renderer_);

        /* Render text overlay */
        renderer_->Draw2DText("Welcome!", 100, 100, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

        renderer_->EndFrame();

        /* End the frame */
        frame_regulator_.FrameEnd();
    }

    void GameEngine::SetCamera(gl::OpenGLCamera * camera) {
        int ret  = renderer_->SetCamera(camera);
        camera_ = camera;
        if (ret) dt::Console(dt::CRITICAL, "Settings camera failed");
    }

    void GameEngine::SetWindowSize(size_t width, size_t height) {

        renderer_->SetWindowSize(width, height);
        camera_->SetPositionVector(0, 0, 8);
    }

    KeyControls_t GameEngine::GetControlsInput() {
        return key_controls_;
    }

    Real_t GameEngine::GetFrameDelta() {
        return frame_regulator_.GetDelta();
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
