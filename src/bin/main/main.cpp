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

    CodeReminder("Create something for random numbers");
    CodeReminder("Create shader class");
    CodeReminder("Deal with ObjLoad files")
    CodeReminder("FrameRateRegulator sleep function");
    CodeReminder("Collision detection, rotation");
    CodeReminder("See: stbi_load to read textures from files");

    ge::OpenGLContextConfig_t context_params;
    context_params.opengl_version_major_ = 3;
    context_params.opengl_version_minor_ = 3;
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
    camera_params.dir_y_ = 0;
    camera_params.dir_z_ = 0;
    camera_params.up_x_ = 0;
    camera_params.up_y_ = 1;
    camera_params.up_z_ = 0;
    camera_params.orthographic_ = false;
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
    engine.AddWorldObject<Grass>(object_tile, texture_grass, -1.0f, 1.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, 0.0f, 1.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, 1.0f, 1.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, -1.0f, 0.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, 0.0f, 0.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, 1.0f, 0.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, -1.0f, -1.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, 0.0f, -1.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, 1.0f, -1.0f);
    engine.AddWorldObject<Grass>(object_tile, texture_grass, -1.0f, 2.0f);
    Treasure * tres = engine.AddWorldObject<Treasure>(object_tile, texture_treasure, 0.0f, 1.5f, 0.1f);
    if (tres != nullptr) tres->SetCollision(1.0f, 1.0f);
    Treasure * tres_1 = engine.AddWorldObject<Treasure>(object_tile, texture_treasure, -2.4f, 1.5f, 0.1f);
    if (tres_1 != nullptr) tres_1->SetCollision(1.0f, 1.0f);
    Treasure * tres_2 = engine.AddWorldObject<Treasure>(object_tile, texture_treasure, 5.0f, 5.0f, 0.1f);
    tres_2->SetCollision(0.5f);

    /* Create a main player */
    Player player;
    player.Init();
    player.SetPosition(0.0f, 0.0f, 0.1f);
    player.SetCollision(1.0f, 1.0f);
    engine.AddMainActor(&player, object_tile, texture_player);

    ge::FrameRateRegulator frame_regulator;
    frame_regulator.Init(100);

    do {
        frame_regulator.FrameStart();
        float delta_time = frame_regulator.GetDelta();

        ControlInput_t input = engine.GetControlsInput();
        if (input.KEY_ESC) {
            break;
        } 
        
        float speed = player.GetSpeed(input.KEY_RUN);
        float move_offset = (1.0f * speed) * delta_time;
        ge::CollisionResult_t can_move = engine.CheckCollision(&player, move_offset, input);
        player.Move(move_offset, input, can_move);
        if (can_move.left_) engine.CameraMove2D(-can_move.left_, 0);
        if (can_move.right_) engine.CameraMove2D(can_move.right_, 0);
        if (can_move.up_) engine.CameraMove2D(0, can_move.up_);
        if (can_move.down_) engine.CameraMove2D(0, -can_move.down_);

        if (input.KEY_PAGE_UP) engine.CameraZoom2D(20 * delta_time);
        if (input.KEY_PAGE_DOWN) engine.CameraZoom2D(-20 * delta_time);

        engine.Step(delta_time);

        frame_regulator.FrameEnd();
        displayFPS(frame_regulator.GetDelta() * 1000.0);
    } while (1);

    frame_regulator.Destroy();
    engine.Destroy();
}