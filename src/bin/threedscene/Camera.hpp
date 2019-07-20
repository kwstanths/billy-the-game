#ifndef __Camera_hpp__
#define __Camera_hpp__

#include "game_engine/math/Types.hpp"
#include "game_engine/graphics/opengl/OpenGLCamera.hpp"
#include "game_engine/graphics/opengl/OpenGLIncludes.hpp"

namespace ge = game_engine;

/**
    Callback function when the mouse moves
*/
void MouseCallback(GLFWwindow * w, double x, double y);

/**
    Callback function where nothing happens when mouse moves
*/
void MouseCallbackNONE(GLFWwindow * w, double x, double y);


class Camera : public game_engine::graphics::opengl::OpenGLCamera {
public:
    Camera(int width, int height, ge::Real_t mouse_sensitivity);

    /** 
         
    */
    void KeyboardMove(ge::Real_t x, ge::Real_t y, ge::Real_t z);

    /**

    */
    void KeyboardMoveFlightMode(ge::Real_t front, ge::Real_t left);

    /**
        FPS Style movement
    */
    void MouseMove(ge::Real_t mouse_x, ge::Real_t mouse_y);

    /**
    
    */
    void Set2DPosition(ge::Real_t x, ge::Real_t y);

private:
    bool is_inited_;
    bool first_mouse_call_;

    ge::Real_t last_cursor_position_x_, last_cursor_position_y_;
    ge::Real_t pitch_, yaw_;
    ge::Real_t mouse_sensitivity_;
    ge::Real_t z_height_;
};

/* Global variable Camera */
extern Camera * camera;


#endif