#ifndef __GameEngine_hpp__
#define __GameEngine_hpp__

#include <string>
#include <vector>

#include "opengl/OpenGLContext.hpp"
#include "opengl/OpenGLCamera.hpp"
#include "opengl/OpenGLRenderer.hpp"
#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"

#include "FrameRateRegulator.hpp"
#include "Controls.hpp"
#include "WorldObject.hpp"
#include "WorldSector.hpp"
#include "AssetManager.hpp"
#include "Debugger.hpp"
#include "ErrorCodes.hpp"


namespace game_engine {

    /**
        Values necessary to initialize a GameEngine object
    */
    typedef struct {
        OpenGLContextConfig_t context_params_;
        OpenGLCameraConfig_t camera_params_;

        size_t frame_rate_;

    } GameEngineConfig_t;

    class GameEngine {
        friend class WorldSector;
    public:
        /**
            Does nothing in particular
        */
        GameEngine();

        /**
            Deallocates the objects used. DOES NOT call their respective Destroy() method
            Objects should be deallocated through Destroy()
        */
        ~GameEngine();

        /**
            Does Engine initialization using the configuration given in the constructor
            @return 0=OK, -1=Not initialzed, anything else from ErrorCodes.hpp
        */
        int Init(GameEngineConfig_t config);

        /**
            Terminates and deallocates the window
            @return 0=OK, -1=Not initialized
        */
        int Destroy();

        /**
            Check whether the game engine has been initialised
            @return is initialised
        */
        bool IsInited();

        /**
            A single engine step, Should be called inside the main loop
        */
        void Step(double delta_time);

        /**
            Get a struct with input from the user
            @return A ControlInput_t struct
        */
        KeyControls_t GetControlsInput();

        /**
            Get the delta of the last frame in seconds
            @return Frame delta in seconds
        */
        float GetFrameDelta();

        /**
            Moves the camera initilised in a 2D manner. Changes of the camera's position will be reflected to the
            camera's direction as well. See OpenGLCamera::Move()
            @param move_x Displacement in X axis
            @param move_y Displacement in Y axis
            @param move_z Displacement in Z axis
        */
        void CameraMove2D(float move_x, float move_y);

        /**
            Zooms into an orthographic camera
            @param zoom_factor The zoom factor
        */
        void CameraZoom2D(float zoom_factor);

        /**
            Set a world sector to be used
            @return 0 = OK, Else see ErrorCodes.hpp
        */
        int SetWorld(WorldSector * world);

        /**
            Get the asset manager
            @return The asset manager. nullptr is returned if Init() error
        */
        AssetManager * GetAssetManager();

        /**
            Get the debugger object
            @return The debugger. nullptr is returned if Init() error
        */
        Debugger * GetDebugger();

        /**
            Get the last error occured, 0 = No error
            @return The last error
        */
        int GetLastError();

        /**
            Stops nothing and exits
        */
        void Terminate();

    private:
        bool is_inited_;
        int last_error_;
        GameEngineConfig_t config_;
        KeyControls_t key_controls_;

        FrameRateRegulator frame_regulator_;
        OpenGLContext * context_ = nullptr;
        OpenGLCamera * camera_ = nullptr;
        OpenGLRenderer * renderer_ = nullptr;
        AssetManager * asset_manager_ = nullptr;
        Debugger * debugger_ = nullptr;

        WorldSector * sector_;
        std::vector<WorldObject *> visible_world_;

    };

}

#endif
