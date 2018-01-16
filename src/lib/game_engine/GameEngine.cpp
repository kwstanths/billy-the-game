#include "GameEngine.hpp"

#include "ErrorCodes.hpp"
#include "physics/Types.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine {

    GameEngine::GameEngine(OpenGLContextConfig_t & context_params, OpenGLCameraConfig_t & camera_params) {
        is_inited_ = false;
        last_error_ = 0;

        CodeReminder("Game engine FPS");

        context_ = new OpenGLContext(context_params);
        camera_ = new OpenGLCamera(camera_params);
        renderer_ = new OpenGLRenderer();
        sector_ = new WorldSector();
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
        if (is_inited_) {
            last_error_ = -1;
            return -1;
        }
        
        int ret = context_->Init();
        if (ret != 0) {
            last_error_ = ret;
            PrintError(ret);
            Terminate();
        }

        camera_->Init(context_);
        
        renderer_->Init(context_);

        sector_->Init(500, 500, -500.0f, 500.0f, -500.0f, 500.0f, 500 * 500);

        CodeReminder("Find the size and margin of the visible world based on the camera");
        visible_world_ = std::vector<WorldObject *>(200);

        CodeReminder("Support key remapping");

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

        is_inited_ = false;
        last_error_ = 0;
        return last_error_;
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

        renderer_->Draw2DText("Welcome!", 100, 100, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

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

    int GameEngine::AddMainActor(WorldObject * object, OpenGLObject * globject, OpenGLTexture * gltexture) {
        object->Init(globject, gltexture, renderer_);
        main_actor_ = object;
        return 0;
    }

    CollisionResult_t GameEngine::CheckCollision(WorldObject * moving_object, float move_offset, ControlInput input) {
        CollisionResult_t collision;

        /* Find the moving direction based on the input */
        Direction direction;
        if (input.KEY_UP && input.KEY_LEFT) direction = 45.0f;
        else if (input.KEY_LEFT && input.KEY_DOWN) direction = 135.0f;
        else if (input.KEY_RIGHT && input.KEY_UP) direction = 345.0f;
        else if (input.KEY_RIGHT && input.KEY_DOWN) direction = 225.0f;
        else if (input.KEY_UP) direction = 0.0f;
        else if (input.KEY_LEFT) direction = 90.0f;
        else if (input.KEY_RIGHT) direction = 270.0f;
        else if (input.KEY_DOWN) direction = 180.0f;
        else return collision;

        /* TODO find the appropriate sector */
        return sector_->CheckCollision(moving_object, move_offset, direction);
    }

    int GameEngine::GetLastError() {
        return last_error_;
    }

    void GameEngine::Terminate() {
        dt::WaitInput();
        exit(-1);
    }

}
