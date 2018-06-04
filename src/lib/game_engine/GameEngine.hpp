#ifndef __GameEngine_hpp__
#define __GameEngine_hpp__

#include <string>
#include <vector>

#include "opengl/OpenGLContext.hpp"
#include "opengl/OpenGLCamera.hpp"
#include "opengl/OpenGLRenderer.hpp"
#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"

#include "ControlInput.hpp"
#include "WorldObject.hpp"
#include "WorldSector.hpp"
#include "AssetManager.hpp"
#include "ErrorCodes.hpp"


namespace game_engine {

    class GameEngine {
    public:
        /**
            Does nothing in particular
        */
        GameEngine(OpenGLContextConfig_t & context_params, OpenGLCameraConfig_t & camera_params);

        /**
            Deallocates the objects used. DOES NOT call their respective Destroy() method
            Objects should be deallocated through Destroy()
        */
        ~GameEngine();

        /**
            Does Engine initialization using the configuration given in the constructor
            @return 0=OK, -1=Not initialzed, anything else from ErrorCodes.hpp
        */
        int Init();

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
        ControlInput_t GetControlsInput();

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

        OpenGLContext * context_ = nullptr;
        OpenGLCamera * camera_ = nullptr;
        OpenGLRenderer * renderer_ = nullptr;
        AssetManager * asset_manager_ = nullptr;

        WorldSector * sector_;
        std::vector<WorldObject *> visible_world_;

    };

}

#endif
