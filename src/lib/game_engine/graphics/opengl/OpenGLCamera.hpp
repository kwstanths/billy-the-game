#ifndef __OpenGLCamera_hpp__
#define __OpenGLCamera_hpp__

#include <glm/glm.hpp>

#include "game_engine/math/Types.hpp"

#include "OpenGLIncludes.hpp"
#include "OpenGLShaders.hpp"
#include "OpenGLContext.hpp"

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
        Real_t zoom_factor_;
        /* z near, z far */
        Real_t z_near_, z_far_;
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
        void GetPositionVector(Real_t & pos_x, Real_t & pos_y, Real_t & pos_z);
        glm::vec3 GetPositionVector();


        /**
            Get camera's direction vector
            @param[out] dir_x X
            @param[out] dir_y Y
            @param[out] dir_z Z
        */
        void GetDirectionVector(Real_t & dir_x, Real_t & dir_y, Real_t & dir_z);
        glm::vec3 GetDirectionVector();

        /**
            Get camera's up vector
            @param[out] up_x X
            @param[out] up_y Y
            @param[out] up_z Z
        */
        void GetUpVector(Real_t & up_x, Real_t & up_y, Real_t & up_z);
        glm::vec3 GetUpVector();

        /**
            
        */
        Real_t GetPerspectiveAngle();

        /**
            Set camera's position vector
            @param x x coordinate
            @param y y coordinate
            @param z z coordinate
        */
        void SetPositionVector(Real_t x, Real_t y, Real_t z);

        /**
            Set camera's direction vector
            @param x x coordinate
            @param y y coordinate
            @param z z coordinate
        */
        void SetDirectionVector(Real_t x, Real_t y, Real_t z);

        /**
            Set camera's up vector
            @param x x coordinate
            @param y y coordinate
            @param z z coordinate
        */
        void SetUpVector(Real_t x, Real_t y, Real_t z);

        /**
            Move the camera's position
            @param move_x Movement in x
            @param move_y Movement in y
            @param move_z Movement in z
        */
        void MovePositionVector(Real_t move_x, Real_t move_y, Real_t move_z);

        /**
            Move the camera's directon
            @param move_x Movement in x
            @param move_y Movement in y
            @param move_z Movement in z
        */
        void MoveDirectionVector(Real_t move_x, Real_t move_y, Real_t move_z);

        /**
            Zoom in/out of the scene
            @param factor Add to the camera zoom_factor already set
        */
        void Zoom(Real_t factor);

        /**
            Set a function for callback when the mouse moves
        */
        int SetMouceCallback(void(*func)(GLFWwindow *, double, double));

        /**

        */
        glm::mat4 GetViewMatrix();

        /**

        */
        glm::mat4 GetProjectionMatrix();

    protected:

        OpenGLCameraConfig_t config_;
    
    private:
        bool is_inited_;
        float perspective_angle_ = glm::radians(45.0f);

        OpenGLContext * context_;

        glm::mat4 view_matrix_;
        glm::mat4 projection_matrix_;
        
        
        /**
            Set Orthographic projection matrix
        */
        void Ortho2D(Real_t zoom_factor);

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