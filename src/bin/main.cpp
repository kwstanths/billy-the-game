#ifdef _WIN32
#include <Windows.h>
#endif

#include "debug_tools/CodeReminder.hpp"

#include "game_engine/GameEngine.hpp"
#include "game_engine/ControlInput.hpp"
#include "game_engine/FrameRateRegulator.hpp"
#include "game_engine/GameWorld.hpp"

#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "Player.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;

//void displayFPS(double frame_time_ms) {
//
//    /* That's so nasty */
//    static double start;
//    static unsigned int frames;
//
//    start += frame_time_ms;
//    frames++;
//    if (start >= 1000.0f) {
//       
//        std::cout << "FPS: " << frames << std::endl;
//
//        start = 0.0;
//        frames = 0;
//    }
//
//}

int main(int argc, char ** argv) {

    CodeReminder("game world class in main add objects");

    ge::OpenGLContextConfig_t context_params;
    context_params.opengl_version_major_ = 3;
    context_params.opengl_version_minor_ = 3;
    context_params.window_width_ = 1024;
    context_params.window_height_ = 768;
    context_params.window_name_ = "billy";
    context_params.shader_vertex_file_path_ = "shaders/VertexShader.glsl";
    context_params.shader_fragment_file_path_ = "shaders/FragmentShader.glsl";
    ge::OpenGLCameraConfig_t camera_params;
    camera_params.pos_x_ = 0;
    camera_params.pos_y_ = 0;
    camera_params.pos_z_ = 10;
    camera_params.dir_x_ = 0;
    camera_params.dir_y_ = 0;
    camera_params.dir_z_ = 0;
    camera_params.up_x_ = 0;
    camera_params.up_y_ = 1;
    camera_params.up_z_ = 0;
    camera_params.orthographic_ = true;
    camera_params.zoom_factor_ = 75;
    ge::GameEngine engine(context_params, camera_params);
    engine.Init();
    
    /* Initialize assets */
    ge::OpenGLObject * object_cube = new ge::OpenGLObject();
    object_cube->Init("assets/cube.obj");
    ge::OpenGLObject * object_grass = new ge::OpenGLObject();
    object_grass->Init("assets/grass.obj");

    ge::OpenGLTexture * texture_cube = new ge::OpenGLTexture();
    texture_cube->Init("assets/uvmap_cube.DDS", ge::OpenGLTexture::TEXTURE_DDS);
    ge::OpenGLTexture * texture_grass = new ge::OpenGLTexture();
    texture_grass->Init("assets/grass.bmp", ge::OpenGLTexture::TEXTURE_BMP);


    /* Create some dummy objects */
    std::vector<Player> some_objects;
    some_objects.resize(2);

    /* Object number 1*/
    some_objects[0].Init();
    some_objects[0].SetPosition(0.0f, 0.0f, 0.0f);
    engine.AddObject(&(some_objects[0]), object_cube, texture_cube);

    /* Object number 2*/
    some_objects[1].Init();
    some_objects[1].SetPosition(2.5f, 2.5f, 0.0f);
    engine.AddObject(&(some_objects[1]), object_grass, texture_grass);

    ge::FrameRateRegulator frame_regulator;
    frame_regulator.Init(30);

    do {
        frame_regulator.FrameStart();
        double delta_time = frame_regulator.GetDelta();

        ControlInput_t input = engine.GetControlsInput();
        if (input.KEY_ESC) {
            break;
        } 
        if (input.KEY_LEFT) {
            engine.CameraMove2D(-4 * delta_time, 0);
            some_objects[0].Move(-4 * delta_time, 0);
        }
        if (input.KEY_RIGHT) {
            engine.CameraMove2D(4 * delta_time, 0);
            some_objects[0].Move(4 * delta_time, 0);
        }
        if (input.KEY_UP) {
            engine.CameraMove2D(0, 4 * delta_time);
            some_objects[0].Move(0, 4 * delta_time);
        }
        if (input.KEY_DOWN) {
            engine.CameraMove2D(0, -4 * delta_time);
            some_objects[0].Move(0, -4 * delta_time);
        }
        if (input.KEY_PAGE_UP) engine.CameraZoom2D(20 * delta_time);
        if (input.KEY_PAGE_DOWN) engine.CameraZoom2D(-20 * delta_time);


        engine.Step(delta_time);



        frame_regulator.FrameEnd();
    } while (1);

    frame_regulator.Destroy();
    engine.Destroy();
}