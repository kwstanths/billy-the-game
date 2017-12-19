#ifndef __WorldObject_hpp__
#define __WorldObject_hpp__

#include <functional>

#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"
#include "opengl/OpenGLRenderer.hpp"

#include "glm/glm.hpp"

namespace game_engine {

    typedef struct {

    } WorldObjectVars_t;

    /**
        A world object is an entity in the world. Override this class, call the function
        Init(... , ...), and provide your custom behaviour in the Step() function. Step()
        should be overriden
    */
    class WorldObject {
    public:
        /**
            Does nothing
        */
        WorldObject();

        /**
            Initializes the position, and calls the OpenGLObject::Init()
        */
        int Init(OpenGLObject * object, OpenGLTexture * texture, OpenGLRenderer * renderer);

        /**
            Calls the OpenGLObject::Destroy()
        */
        int Destroy();

        /**
            Get wether or not the object is initialised
        */
        bool IsInited();

        /**
            Draw the object using the variables set in Init(...);
        */
        void Draw();

        /**
            Function that should be overriden
        */
        virtual void Step(double delta_time);

        /**
            Set the model matrix for the object
            @param pos_x The model's position x
            @param pos_y The model's position y
            @param pos_z The model's position z
        */
        void SetPosition(float pos_x, float pos_y, float pos_z);

        /**
            Set a function to be executed every time Step is called
        */
        void SetStepFunction(std::function<void(double)> func);

    protected:
        float pos_x_, pos_y_, pos_z_;

    private:
        bool is_inited_;

        OpenGLObject * object_;
        OpenGLTexture * texture_;
        OpenGLRenderer * renderer_;
        glm::mat4 model_;

        std::function<void(double)> func_;
    };
}
#endif