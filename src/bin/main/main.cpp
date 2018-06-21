#ifdef _WIN32
#include <Windows.h>
#endif

#include "game_engine/GameEngine.hpp"
#include "game_engine/physics/Types.hpp"

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"

#include "Input.hpp"
#include "Camera.hpp"
#include "World.hpp"

namespace ge = game_engine;
namespace gl = game_engine::graphics::opengl;
namespace dt = debug_tools;

/* Camera needs to be global, since the mouse callback needs to be global */
Camera * camera;

void MouseCallback(GLFWwindow * w, double x, double y) {
    camera->MouseMove(x, y);
}

int main(int argc, char ** argv) {

    CodeReminder("Move the opengl initialization from the game engine to the renderer");
    CodeReminder("Rendering order");
    CodeReminder("Draw with different shader");
    CodeReminder("Use the interactable property");
    CodeReminder("WorldObject, collision in SetPosition() maybe?");

    /* Configuration parameters for the engine */
    gl::OpenGLContextConfig_t context_params;
    context_params.window_width_ = 1024;
    context_params.window_height_ = 768;
    context_params.window_name_ = "billy";
    context_params.font_file_path = "fonts/arial.ttf";
    context_params.shader_vertex_file_path_ = "shaders/VertexShader.glsl";
    context_params.shader_fragment_file_path_ = "shaders/FragmentShader.glsl";
    context_params.shader_vertex_text_file_path = "shaders/TextVertexShader.glsl";
    context_params.shader_fragment_text_file_path = "shaders/TextFragmentShader.glsl";
    gl::OpenGLCameraConfig_t camera_params;
    camera_params.position_ = glm::vec3(0, 0, 8);
    camera_params.direction_ = glm::vec3(0, 0, -1);
    camera_params.up_ = glm::vec3(0, 1, 0);
    camera_params.orthographic_ = false;
    camera_params.zoom_factor_ = 75;
    ge::GameEngineConfig_t engine_params;
    engine_params.context_params_ = context_params;
    engine_params.frame_rate_ = 100;
    ge::GameEngine engine;
    if (engine.Init(engine_params)) return false;
    
    /* Create a camera */
    camera = new Camera(context_params.window_width_, context_params.window_height_, 0.05f);
    engine.SetCamera(camera, camera_params);
    camera->SetMouceCallback(MouseCallback);
    
    /* Initialize the input class */
    Input input;
    input.Init(&engine);

    /* Create a world */
    World world;
    world.Init(&input, camera, &engine);

    /* Set the active world in the engine */
    engine.SetWorld(&world);
    do {
        float delta_time = engine.GetFrameDelta();
        
        ControlInput_t controls = input.GetControls();
        if (controls.QUIT_) {
            break;
        }

        if (controls.ZOOM_IN_) camera->Zoom(-10 * delta_time);
        if (controls.ZOOM_OUT_) camera->Zoom(10 * delta_time);

        engine.Step(delta_time);

    } while (1);

    engine.Destroy();
}