#ifndef __Camera_hpp__
#define __Camera_hpp__


#include "game_engine/opengl/OpenGLCamera.hpp"


class Camera : public game_engine::OpenGLCamera {
public:
    Camera(int width, int height, float mouse_sensitivity);

    /** 
        Moves the camera in a 2d manner 
    */
    void KeyboardMove(float x, float y, float z);

    /**
    
    */
    void MouseMove(float mouse_x, float mouse_y);

private:
    bool is_inited_;
    float last_cursor_position_x_, last_cursor_position_y_;
    float pitch_, yaw_;
    float mouse_sensitivity_;

};


#endif