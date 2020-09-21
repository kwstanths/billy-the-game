#ifndef __GameEngine_hpp__
#define __GameEngine_hpp__

#include <string>
#include <vector>

#include "game_engine/graphics/opengl/OpenGLCamera.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"

#include "FrameRateRegulator.hpp"
#include "Controls.hpp"
#include "WorldObject.hpp"
#include "WorldSector.hpp"
#include "Debugger.hpp"
#include "ErrorCodes.hpp"


namespace game_engine {

    /**
        Values necessary to initialize a GameEngine object
    */
    typedef struct {
        graphics::opengl::OpenGLContextConfig_t context_params_;

        size_t frame_rate_;

    } GameEngineConfig_t;

    class GameEngine {
        friend WorldSector;
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
        
        */
        void SetCamera(graphics::opengl::OpenGLCamera * camera);

        graphics::opengl::OpenGLCamera * GetCamera();

        void SetWindowSize(size_t width, size_t height);

        /**
            Get a struct with input from the user
            @return A ControlInput_t struct
        */
        KeyControls_t GetControlsInput();

        /**
            Get the delta of the last frame in seconds
            @return Frame delta in seconds
        */
        Real_t GetFrameDelta();

        /**
            Set a world sector to be used
            @return 0 = OK, Else see ErrorCodes.hpp
        */
        int SetWorld(WorldSector * world);

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
        size_t fps_;

        /* Engine configuration values */
        GameEngineConfig_t config_;
        /* Latest keybaord values pressed. Updated at every engine step */
        KeyControls_t key_controls_;
        
        /* Instances from other parts of the system */
        FrameRateRegulator frame_regulator_;
        Debugger * debugger_ = nullptr;
        WorldSector * sector_;
        
        graphics::Renderer * renderer_ = nullptr;
        graphics::opengl::OpenGLCamera * camera_ = nullptr;

        void MeasureFPS(double frame_time_ms);
    };

}

#endif
