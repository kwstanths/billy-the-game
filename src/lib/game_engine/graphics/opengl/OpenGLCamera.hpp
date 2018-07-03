#ifndef __OpenGLCamera_hpp__
#define __OpenGLCamera_hpp__

#include "OpenGLIncludes.hpp"
#include "OpenGLShaders.hpp"
#include "OpenGLContext.hpp"

#include <glm/glm.hpp>

namespace game_engine {
namespace graphics {
namespace opengl {

    /**
        Values necessary to initialize an OpenGLCamera object
    */
    typedef struct {
        /* Camera position */
        glm::vec3 position_;
        /* Camera view direction */
        glm::vec3 direction_;
        /* Camera up vector */
        glm::vec3 up_;
        /* true: Orthographic-2D, false: Perspective-3D */
        bool orthographic_;
        /* If orthigraphic is applied, set the zoom factor */
        float zoom_factor_;
    } OpenGLCameraConfig_t;


    class OpenGLCamera {
        friend class OpenGLContext;
        friend class OpenGLRenderer;
    public:
        /**
            Sets the parameters to the local variable, does nothing else. Feel free to discard
            the config object after this call
            @param config The necessary stuff when Init() is called
        */
        OpenGLCamera();

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
            Get camera's position
            @param[out] pos_x X
            @param[out] pos_y Y
            @param[out] pos_z Z
        */
        void GetPositionVector(float * pos_x, float * pos_y, float * pos_z);

        /**
            Get camera's direction vector
            @param[out] dir_x X
            @param[out] dir_y Y
            @param[out] dir_z Z
        */
        void GetDirectionVector(float * dir_x, float * dir_y, float * dir_z);

        /**
            Get camera's up vector
            @param[out] up_x X
            @param[out] up_y Y
            @param[out] up_z Z
        */
        void GetUpVector(float * up_x, float * up_y, float * up_z);

        /**
            Set camera's position vector
            @param x x coordinate
            @param y y coordinate
            @param z z coordinate
        */
        void SetPositionVector(float x, float y, float z);

        /**
            Set camera's direction vector
            @param x x coordinate
            @param y y coordinate
            @param z z coordinate
        */
        void SetDirectionVector(float x, float y, float z);

        /**
            Set camera's up vector
            @param x x coordinate
            @param y y coordinate
            @param z z coordinate
        */
        void SetUpVector(float x, float y, float z);

        /**
            Move the camera's position
            @param move_x Movement in x
            @param move_y Movement in y
            @param move_z Movement in z
        */
        void MovePositionVector(float move_x, float move_y, float move_z);

        /**
            Move the camera's directon
            @param move_x Movement in x
            @param move_y Movement in y
            @param move_z Movement in z
        */
        void MoveDirectionVector(float move_x, float move_y, float move_z);

        /**
            Zoom in/out of the scene
            @param factor Add to the camera zoom_factor already set
        */
        void Zoom(float factor);

        /**
            Set a function for callback when the mouse moves
        */
        int SetMouceCallback(void(*func)(GLFWwindow *, double, double));

    protected:

        OpenGLCameraConfig_t config_;
    
    private:
        bool is_inited_;

        OpenGLContext * context_;

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


        /**
            Calulcate the projection matrix
        */
        int CalculateView();

    };


}
}
}

#endif