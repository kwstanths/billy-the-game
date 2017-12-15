#ifndef __GameEngine_hpp__
#define __GameEngine_hpp__

#include <string>
#include <vector>

#include "opengl/OpenGLContext.hpp"
#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLCamera.hpp"

#include "ControlInput.hpp"
#include "WorldObject.hpp"

namespace game_engine {

    class GameEngine {
    public:
        /**
            TODO
        */
        GameEngine(OpenGLContextConfig_t & context_params, OpenGLCameraConfig_t & camera_params);

        /**
            
        */
        ~GameEngine();

        /**
            TODO
            @return 0=OK, -1=Not initialzed, anything else from ErrorCodes
        */
        int Init();

        /**
            Terminates and deallocates window
            @return 0=OK, -1=Not initialized
        */
        int Destroy();

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
            Add a new object to the scene. The game engine will keep a pointer to the object
            Don't delete the object without first removing it from the engine
        */
        int AddObject(WorldObject * obj, OpenGLObjectConfig_t config);

    private:
        bool is_inited_;

        OpenGLContext * context_ = nullptr;
        OpenGLCamera * camera_ = nullptr;

        /* A struct to keep the object */
        std::vector<WorldObject *> objects_;

        void Terminate();

    };

}

#endif