#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/core/GameEngine.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "Maps/WorldBillyMap.hpp"
#include "Maps/WorldTavern1a.hpp"
#include "Maps/WorldTavern1b.hpp"
#include "Maps/WorldHouse1a.hpp"
#include "Maps/WorldHouse1b.hpp"
#include "Player.hpp"
#include "Camera.hpp"

namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;
namespace dt = debug_tools;
namespace math = game_engine::math;


int main(int argc, char ** argv) {

    CodeReminder("Delete all physics things, use some 3rd party library")
    CodeReminder("Continuous memory allocation for Model()");
    CodeReminder("Continuous memory allocation for Mesh()");
    CodeReminder("Continuous memory allocation for Collision objects");
    CodeReminder("Text dialogs");
    CodeReminder("Fix the build with double");
    
    /* Configuration parameters for the engine */
    gl::OpenGLContextConfig_t context_params;
    context_params.window_width_ = 1024;
    context_params.window_height_ = 768;
    context_params.window_name_ = "billy";
    context_params.font_file_path = "fonts/KateCelebration.ttf";
    ge::GameEngineConfig_t engine_params;
    engine_params.context_params_ = context_params;
    engine_params.frame_rate_ = 75;
    ge::GameEngine engine;
    if (engine.Init(engine_params)) return false;
    
    /* Create a camera */
    camera = new Camera(context_params.window_width_, context_params.window_height_, 0.05f);
    engine.SetCamera(camera);
    camera->SetMouceCallback(MouseCallback);

    /* Initialize the input class */
    Input input;
    input.Init(&engine);

    /* Create the world */
    WorldBillyMap world_billy;
    /* And the inside of some houses... */
    WorldTavern1a world_tavern_1a;
    WorldTavern1b world_tavern_1b;
    WorldHouse1a world_house_1a;
    WorldHouse1b world_house_1b;

    world_tavern_1a.Init(&input, camera, &engine, &world_billy, &world_tavern_1b);
    world_tavern_1b.Init(&input, camera, &engine, &world_tavern_1a);
    world_house_1a.Init(&input, camera, &engine, &world_billy, &world_house_1b);
    world_house_1b.Init(&input, camera, &engine, &world_house_1a);

    world_billy.Init(&input, camera, &engine, &world_tavern_1a, &world_house_1a);
    /* Set the active world in the engine */
    engine.SetWorld(&world_billy);
    do {
        float delta_time = engine.GetFrameDelta();
        
        ControlInput_t controls = input.GetControls();
        if (controls.QUIT_) {
            break;
        }

        if (controls.ZOOM_IN_) camera->Zoom(-20 * delta_time);
        if (controls.ZOOM_OUT_) camera->Zoom(20 * delta_time);

        engine.Step(delta_time);

    } while (1);

    engine.Destroy();
}