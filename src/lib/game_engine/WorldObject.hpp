#ifndef __WorldObject_hpp__
#define __WorldObject_hpp__

#include "opengl/OpenGLContext.hpp"
#include "opengl/OpenGLObject.hpp"

namespace game_engine {

    /**
        A world object is an entity in the world. Override this class, call the function
        Init(... , ...), and provide your custom behaviour in the Step() function. Step()
        should be overriden
    */
    class WorldObject : public OpenGLObject{
    public:
        /**
            Does nothing
        */
        WorldObject();

        /**
            Initializes the position, and calls the OpenGLObject::Init()
        */
        int Init(OpenGLObjectConfig_t obj_config, OpenGLContext * context);

        /**
            Calls the OpenGLObject::Destroy()
        */
        int Destroy();

        /**
            Get wether or not the object is initialised
        */
        bool IsInited();

        /**
            Function that should be overriden
        */
        virtual void Step(double delta_time) {};

    private:
        bool is_inited_;
    };
}
#endif