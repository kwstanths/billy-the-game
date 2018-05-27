#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/GameEngine.hpp"
#include "game_engine/ControlInput.hpp"
#include "game_engine/FrameRateRegulator.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/ErrorCodes.hpp"
#include "game_engine/WorldObject.hpp"
#include "game_engine/opengl/OpenGLObject.hpp"
#include "game_engine/opengl/OpenGLTexture.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "Player.hpp"
#include "Grass.hpp"
#include "Treasure.hpp"
#include "World.hpp"

namespace ge = game_engine;
namespace dt = debug_tools;

void displayFPS(double frame_time_ms) {

    /* That's so nasty */
    static double start;
    static unsigned int frames;

    start += frame_time_ms;
    frames++;
    if (start >= 1000.0f) {
       
        std::cout << "FPS: " << frames << std::endl;

        start = 0.0;
        frames = 0;
    }

}


int main(int argc, char ** argv) {

    CodeReminder("Find an accurate Sleep function or some sort of accurate delay");
    CodeReminder("WorldObject, collision in SetPosition");
    CodeReminder("WorldSector, remove array add quad tree")
    CodeReminder("GameEngine, Create shader class");
    CodeReminder("See: stbi_load to read textures from files");

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
    camera_params.pos_z_ = 10;
    camera_params.dir_x_ = 0;
    camera_params.dir_y_ = 1;
    camera_params.dir_z_ = 0;
    camera_params.up_x_ = 0;
    camera_params.up_y_ = 1;
    camera_params.up_z_ = 0;
    camera_params.orthographic_ = false;
    camera_params.zoom_factor_ = 75;
    ge::GameEngine engine(context_params, camera_params);
    engine.Init();
    
    World world;
    world.Init(&engine);

    engine.SetWorld(&world);

    ge::FrameRateRegulator frame_regulator;
    frame_regulator.Init(140);

    do {
        frame_regulator.FrameStart();
        float delta_time = frame_regulator.GetDelta();
        /*dt::ConsoleInfo("DELTA TIME", delta_time * 1000.0f);
        */
        ControlInput_t input = engine.GetControlsInput();
        if (input.KEY_ESC) {
            break;
        }

        if (input.KEY_PAGE_UP) engine.CameraZoom2D(10 * delta_time);
        if (input.KEY_PAGE_DOWN) engine.CameraZoom2D(-10 * delta_time);

        engine.Step(delta_time);

        //dt::ConsoleInfo("Timestamp before", dt::GetMSecondsSinceEpoch());
        frame_regulator.FrameEnd();
        //dt::ConsoleInfo("Timestamp after", dt::GetMSecondsSinceEpoch());

        displayFPS(frame_regulator.GetDelta() * 1000.0);
    } while (1);

    frame_regulator.Destroy();
    engine.Destroy();
}