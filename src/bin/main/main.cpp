#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/GameEngine.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "Input.hpp"
#include "World.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;


int main(int argc, char ** argv) {

    CodeReminder("WorldObject, collision in SetPosition");
    CodeReminder("WorldSector, remove array add quad tree")

    ge::OpenGLContextConfig_t context_params;
    context_params.window_width_ = 1024;
    context_params.window_height_ = 768;
    context_params.window_name_ = "billy";
    context_params.font_file_path = "fonts/arial.ttf";
    context_params.shader_vertex_file_path_ = "shaders/VertexShader.glsl";
    context_params.shader_fragment_file_path_ = "shaders/FragmentShader.glsl";
    context_params.shader_vertex_text_file_path = "shaders/TextVertexShader.glsl";
    context_params.shader_fragment_text_file_path = "shaders/TextFragmentShader.glsl";
    ge::OpenGLCameraConfig_t camera_params;
    camera_params.pos_x_ = 0;
    camera_params.pos_y_ = 0;
    camera_params.pos_z_ = 8;
    camera_params.dir_x_ = 0;
    camera_params.dir_y_ = 1;
    camera_params.dir_z_ = 0;
    camera_params.up_x_ = 0;
    camera_params.up_y_ = 1;
    camera_params.up_z_ = 0;
    camera_params.orthographic_ = false;
    camera_params.zoom_factor_ = 75;
    ge::GameEngineConfig_t engine_params;
    engine_params.context_params_ = context_params;
    engine_params.camera_params_ = camera_params;
    engine_params.frame_rate_ = 100;
    ge::GameEngine engine;
    if (engine.Init(engine_params)) return false;
    
    Input input;
    input.Init(&engine);

    World world;
    world.Init(&input, &engine);

    engine.SetWorld(&world);
    do {
        float delta_time = engine.GetFrameDelta();
        
        ControlInput_t controls = input.GetControls();
        if (controls.QUIT_) {
            break;
        }

        if (controls.ZOOM_IN_) engine.CameraZoom2D(10 * delta_time);
        if (controls.ZOOM_OUT_) engine.CameraZoom2D(-10 * delta_time);

        engine.Step(delta_time);

    } while (1);

    engine.Destroy();
}