#ifndef __OpenGLCamera_hpp__
#define __OpenGLCamera_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLShaderVariables.hpp"
#include "OpenGLContext.hpp"

#include <glm/glm.hpp>

namespace game_engine {

    /**
        Values necessary to initialize an OpenGLCamera object
    */
    typedef struct {
        float pos_x_, pos_y_, pos_z_;
        float dir_x_, dir_y_, dir_z_;
        float up_x_, up_y_, up_z_;
    } OpenGLCameraParams_t;


    class OpenGLCamera {
    public:
        /**
            Sets the parameters to the local variable, does nothing else. Feel free to discard 
            the params object after this call
            @param params The necessary stuff when Init() is called
        */
        OpenGLCamera(OpenGLCameraParams_t params);

        /**
            Uses the parameters set to initialise the View matrix, and the projection matrix.
            Also, sets the OpenGLShaderVariables_t to be used with SetView
            @param context Used to get OpenGLShaderVariables_t object to be used to set the view 
            and projection matrices
            @return 0=OK
                
        */
        int Init(OpenGLContext * context);

        /**
            Init allocates nothing, Destroy deallocates nothing
        */
        int Destroy();

        /**
            Sets a new position for the camera
            @param params The necessary stuff to set the camera position
        */
        void SetPosition(OpenGLCameraParams_t params);

        /**
            Move the camera's position relative the parameters already set. The direction of where the
            camera looks at is changed as well with the same value, in order to maintain the 2D movement
            @param move_x Add to the camera position/direction x, the value move_x
            @param move_y Add to the camera position/direction y, the value move_y
            @param move_z Add to the camera position/direction z, the value move_z
        */
        void Move2D(float move_x, float move_y, float move_z);

        /**
            Sets the view and projection matrix to the shaders
            @return 0=OK, -1=Not initialised
        */
        int SetView();

    private:
        bool is_inited_;
        OpenGLCameraParams_t params_;
        OpenGLShaderVariables_t shader_vars_;

        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;
    };
}

#endif