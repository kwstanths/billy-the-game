#ifndef __WorldObject_hpp__
#define __WorldObject_hpp__

#include <functional>

#include "opengl/OpenGLObject.hpp"
#include "opengl/OpenGLTexture.hpp"
#include "opengl/OpenGLRenderer.hpp"
#include "Collision.hpp"   

#include "glm/glm.hpp"

namespace game_engine {

    /**
        A world object is an entity in the world. Override this class, call the function
        Init(... , ...), and provide your custom behaviour in the Step() function
    */
    class WorldObject {
    public:

        /**
            Does nothing in particular. Sets the position to (0,0,0)
        */
        WorldObject();

        /**
            Sets the OpenGL model and texture to the object. Also sets the renderer to be used
            @return 0=OK, else see ErrorCodes.hpp
        */
        int Init(OpenGLObject * object, OpenGLTexture * texture, OpenGLRenderer * renderer);

        /**
            Does nothing in particular
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
            Get the x coordinate of the object's position
            @return The x coordinate
        */
        float GetXPosition();

        /**
            Get the y coordinate of the object's position
            @return The y coordinate
        */
        float GetYPosition();

        /**
            Get the z coordinate of the object's position
            @return The z coordinate
        */
        float GetZPosition();

        /**
            Set the model matrix for the object
            @param pos_x The model's position x
            @param pos_y The model's position y
            @param pos_z The model's position z
        */
        void SetPosition(float pos_x, float pos_y, float pos_z);

        /**
            Rotate the object
            @param angle The angle of rotation, Can be negative
            @param axis The axis to rotate arounf, 0=X-axis, 1=Y-axis, 2=Z-axis
        */
        void Rotate(float angle, size_t axis);

        /**
            Set a function to be executed every time Step is called
        */
        void SetStepFunction(std::function<void(double)> func);

        /**
            Set the collision type to be used for this object. Default is no collision
            @param config The collision configuration
        */
        void SetCollision(CollisionConfig_t config);

        /**
            Get the collision parameters
            @return The collision configuration
        */
        CollisionConfig_t GetCollision();

    protected:
        float pos_x_, pos_y_, pos_z_;

    private:
        bool is_inited_;

        OpenGLObject * object_;
        OpenGLTexture * texture_;
        OpenGLRenderer * renderer_;
        CollisionConfig_t collision_config_;

        glm::mat4 translation_matrix_;
        glm::mat4 rotation_matrix_;
        glm::mat4 model_;

        std::function<void(double)> func_;
    };
}
#endif