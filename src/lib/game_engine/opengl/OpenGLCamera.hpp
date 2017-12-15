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
        /* Camera position */
        float pos_x_, pos_y_, pos_z_;
        /* Camera view direction */
        float dir_x_, dir_y_, dir_z_;
        /* Camera up vector */
        float up_x_, up_y_, up_z_;
        /* true: Orthographic-2D, false: Perspective-3D */
        bool orthographic_;
        /* If orthigraphic is applied, set the zoom factor */
        float zoom_factor_;
    } OpenGLCameraConfig_t;


    class OpenGLCamera {
    public:
        /**
            Sets the parameters to the local variable, does nothing else. Feel free to discard 
            the config object after this call
            @param config The necessary stuff when Init() is called
        */
        OpenGLCamera(OpenGLCameraConfig_t config);

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
        void SetPosition(OpenGLCameraConfig_t params);

        /**
            Move the camera's position relative the parameters already set. The direction of where the
            camera looks at is changed as well with the same value, in order to maintain the movement.
            If orthographic projection is set, the move_z value must be zero
            @param move_x Add to the camera position/direction x, the value move_x
            @param move_y Add to the camera position/direction y, the value move_y
            @param move_z Add to the camera position/direction z, the value move_z
        */
        void Move(float move_x, float move_y, float move_z);

        /**
            If orthographic projection is set, zoom in/out of the scene
            @param factor Add to the camera zoom_factor already set
        */
        void Zoom(float factor);

        /**
            Sets the view and projection matrix to the shaders
            @return 0=OK, -1=Not initialised
        */
        int SetView();

    private:
        bool is_inited_;
        OpenGLContext * context_;
        OpenGLCameraConfig_t config_;
        OpenGLShaderVariables_t shader_vars_;

        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;

        /**
            Set Orthographic projection matrix
        */
        void Ortho2D(float zoom_factor);

        /**
            Set Perspective-3D projection matrix
        */
        void Project3D();

    };
}

#endif