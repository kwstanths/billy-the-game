#ifdef _WIN32
#include <Windows.h>
#endif

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "game_engine/GameEngine.hpp"
#include "game_engine/ControlInput.hpp"
#include "game_engine/FrameRateRegulator.hpp"
#include "game_engine/WorldSector.hpp"
#include "game_engine/ErrorCodes.hpp"
#include "game_engine/WorldObject.hpp"

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

    CodeReminder("Change asset tile to have width and height equal to 1");
    CodeReminder("WorldSector::GetObjectsWindow borders/margins sanitization");

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
    /* Objects */
    ge::OpenGLObject * object_tile = new ge::OpenGLObject();
    object_tile->Init("assets/tile.obj");
    /* Textures */
    ge::OpenGLTexture * texture_grass = new ge::OpenGLTexture();
    ge::OpenGLTexture * texture_treasure = new ge::OpenGLTexture();
    ge::OpenGLTexture * texture_player = new ge::OpenGLTexture();
    texture_grass->Init("assets/grass.bmp", ge::OpenGLTexture::TEXTURE_BMP);
    texture_treasure->Init("assets/treasure.bmp", ge::OpenGLTexture::TEXTURE_BMP);
    texture_player->Init("assets/player.bmp", ge::OpenGLTexture::TEXTURE_BMP);

    /* Create a dummy world */
    ge::WorldSector sector;
    sector.Init(500, 500, -500.0f, 500.0f, -500.0f, 500.0f, 500 * 500);
    engine.WorldObjectInit(sector.NewObj(-2.0f, 2.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(0.0f, 2.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(2.0f, 2.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(-2.0f, 0.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(0.0f, 0.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(2.0f, 0.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(-2.0f, -2.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(0.0f, -2.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(2.0f, -2.0f), object_tile, texture_grass);
    engine.WorldObjectInit(sector.NewObj(0.0f, 3.0f, 0.1f), object_tile, texture_treasure);
    
    engine.AddWorldSector(&sector);

    /* Create a main player */
    Player player;
    player.Init();
    player.SetPosition(0.0f, 0.0f, 0.1f);
    engine.WorldObjectInit(&player, object_tile, texture_player);

    engine.AddMainActor(&player);

    ge::FrameRateRegulator frame_regulator;
    frame_regulator.Init(60);

    do {
        frame_regulator.FrameStart();
        double delta_time = frame_regulator.GetDelta();

        ControlInput_t input = engine.GetControlsInput();
        if (input.KEY_ESC) {
            break;
        } 
        float speed = player.GetSpeed(input.KEY_RUN);
        if (input.KEY_LEFT) {
            engine.CameraMove2D(-speed * delta_time, 0);
            player.Move(-speed * delta_time, 0);
        }
        if (input.KEY_RIGHT) {
            engine.CameraMove2D(speed * delta_time, 0);
            player.Move(speed * delta_time, 0);
        }
        if (input.KEY_UP) {
            engine.CameraMove2D(0, speed * delta_time);
            player.Move(0, speed * delta_time);
        }
        if (input.KEY_DOWN) {
            engine.CameraMove2D(0, -speed * delta_time);
            player.Move(0, -speed * delta_time);
        }
        if (input.KEY_PAGE_UP) engine.CameraZoom2D(20 * delta_time);
        if (input.KEY_PAGE_DOWN) engine.CameraZoom2D(-20 * delta_time);

        engine.Step(delta_time);

        frame_regulator.FrameEnd();
    } while (1);

    frame_regulator.Destroy();
    engine.Destroy();
}