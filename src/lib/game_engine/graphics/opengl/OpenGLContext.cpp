#include "OpenGLContext.hpp"

#include "game_engine/core/ErrorCodes.hpp"
#include "game_engine/core/FileSystem.hpp"

#include "debug_tools/CodeReminder.hpp"

namespace dt = debug_tools;

namespace game_engine { namespace graphics { namespace opengl {

    OpenGLContext::OpenGLContext() {
        is_inited_ = false;
    }
    
    int OpenGLContext::Init(OpenGLContextConfig_t config) {
        if (is_inited_) return -1;
    
        config_ = config;
    
        GLfloat window_ratio_ = (config_.window_width_ * 1.0f) / (config_.window_height_ * 1.0f);
    
        /* Initialise GLFW */
        if (!glfwInit()) {
            return Error::ERROR_GLFW_INIT;
        }
    
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                /* To make MacOS happy; should not be needed */
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      /* We don't want old OpenGL */
    
                                                                            /* Open a window and create its OpenGL context */
        glfw_window_ = glfwCreateWindow(config_.window_width_, config_.window_height_, config_.window_name_.c_str(), NULL, NULL);
        if (glfw_window_ == NULL) {
            glfwTerminate();
            return Error::ERROR_GLFW_WINDOW;
        }
    
        glfwSetWindowPos(glfw_window_, 20, 5);
    
        /* Initialize GLEW */
        glfwMakeContextCurrent(glfw_window_);
        glewExperimental = true; /* Needed for core profile */
        if (glewInit() != GLEW_OK) {
            glfwTerminate();
            return Error::ERROR_GLEW_INIT;
        }
    
        glViewport(0, 0, config_.window_width_, config_.window_height_);
        glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
        /* Enable depth test */
        glEnable(GL_DEPTH_TEST);
        /* Accept a fragment if it's closer to the camera than the former one */
        glDepthFunc(GL_LESS);
        /* Discard triangles whose normal is not facing towards the camera */
        glEnable(GL_CULL_FACE);
    
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
        std::string shaders_dir = FileSystem::GetInstance().GetDirectoryShaders();

        /* Compile and link shaders */
        int ret = 0;
        ret += shader_text_.Init(shaders_dir + "/VertexShaderText.glsl", shaders_dir + "/FragmentShaderText.glsl");
        ret += shader_vertices_color_.Init(shaders_dir + "/VertexShaderVerticesColor.glsl", shaders_dir + "/FragmentShaderVerticesColor.glsl");
        ret += shader_quad_.Init(shaders_dir + "/VertexShaderQuad.glsl", shaders_dir + "/FragmentShaderQuad.glsl");
        ret += shader_gbuffer_.Init(shaders_dir + "/VertexShaderGBuffer.glsl", shaders_dir + "/FragmentShaderGBuffer.glsl");
        ret += shader_standard_.Init(shaders_dir + "/VertexShaderGBuffer.glsl", shaders_dir + "/FragmentShaderStandard.glsl");
        ret += shader_ssao_.Init(shaders_dir + "/VertexShaderQuad.glsl", shaders_dir + "/PostProcessing/FragmentShaderSSAO.glsl");
        ret += shader_separable_ao_.Init(shaders_dir + "/VertexShaderQuad.glsl", shaders_dir + "/PostProcessing/FragmentShaderSeparableAO.glsl");
        ret += shader_blur_.Init(shaders_dir + "/VertexShaderQuad.glsl", shaders_dir + "/PostProcessing/FragmentShaderBlur.glsl");
        ret += shader_final_pass_.Init(shaders_dir + "/VertexShaderQuad.glsl", shaders_dir + "/FragmentShaderFinalPass.glsl");
        ret += shader_shadow_map_.Init(shaders_dir + "/VertexShaderShadowMap.glsl", shaders_dir + "/FragmentShaderShadowMap.glsl");
        ret += shader_draw_normals_.Init(shaders_dir + "/VertexShaderDrawNormals.glsl", shaders_dir + "/FragmentShaderDrawNormals.glsl", shaders_dir + "/GeometryShaderDrawNormals.glsl");
        ret += shader_displacement_.Init(shaders_dir + "/Terrain/VertexShaderTessellation.glsl", shaders_dir + "/Terrain/FragmentShaderDisplacement.glsl", shaders_dir + "/Terrain/TessellationControlShaderDisplacement.glsl", shaders_dir + "/Terrain/TessellationEvaluationShaderDisplacement.glsl");
        ret += shader_displacement_draw_normals_.Init(shaders_dir + "/Terrain/VertexShaderTessellation.glsl", shaders_dir + "/Terrain/FragmentShaderDisplacementDrawNormals.glsl", shaders_dir + "/Terrain/TessellationControlShaderDisplacement.glsl", shaders_dir + "/Terrain/TessellationEvaluationShaderDisplacementDrawNormals.glsl", shaders_dir + "/Terrain/GeometryShaderDisplacementDrawNormals.glsl");
        ret += shader_water_.Init(shaders_dir + "/Terrain/VertexShaderTessellation.glsl", shaders_dir + "/Terrain/FragmentShaderWater.glsl", shaders_dir + "/Terrain/TessellationControlShaderDisplacement.glsl", shaders_dir + "/Terrain/TessellationEvaluationShaderWater.glsl");
        ret += shader_skybox_.Init(shaders_dir + "/VertexShaderSkybox.glsl", shaders_dir + "/FragmentShaderSkybox.glsl");
        if (ret) dt::Console(dt::CRITICAL, "Shaders compilation failed");
        
        is_inited_ = true;
        return 0;
    }
    
    bool OpenGLContext::IsInited() {
        return is_inited_;
    }
    
    size_t OpenGLContext::GetWindowWidth() {
        if (!is_inited_) return 0;
    
        return config_.window_width_;
    }
    
    size_t OpenGLContext::GetWindowHeight() {
        if (!is_inited_) return false;
    
        return config_.window_height_;
    }
    
    int OpenGLContext::Destroy() {
        if (!is_inited_) return -1;
    
    
        glfwTerminate();
        glfw_window_ = NULL;
    
        is_inited_ = false;
        return 0;
    }
    
    void OpenGLContext::SetWindowSize(size_t width, size_t height) {
    
        config_.window_height_ = height;
        config_.window_width_ = width;
        GLfloat window_ratio_ = (config_.window_width_ * 1.0f) / (config_.window_height_ * 1.0f);
        glViewport(0, 0, config_.window_width_, config_.window_height_);
    
        glfwSetWindowSize(glfw_window_, width, height);
    }
    
    KeyControls_t OpenGLContext::GetControlsInput() {
        if (!is_inited_) return KeyControls_t();
    
        KeyControls_t key_controls;
    
        if (glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) key_controls.KEY_ESC = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_ENTER) == GLFW_PRESS) key_controls.KEY_ENTER = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) key_controls.KEY_LSHIFT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) key_controls.KEY_LALT = true;
    
        if (glfwGetKey(glfw_window_, GLFW_KEY_A) == GLFW_PRESS) key_controls.KEY_A = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_S) == GLFW_PRESS) key_controls.KEY_S = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_D) == GLFW_PRESS) key_controls.KEY_D = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_W) == GLFW_PRESS) key_controls.KEY_W = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_E) == GLFW_PRESS) key_controls.KEY_E = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_F) == GLFW_PRESS) key_controls.KEY_F = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_O) == GLFW_PRESS) key_controls.KEY_O = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_R) == GLFW_PRESS) key_controls.KEY_R = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_V) == GLFW_PRESS) key_controls.KEY_V = true;
    
        if (glfwGetKey(glfw_window_, GLFW_KEY_UP) == GLFW_PRESS) key_controls.KEY_ARROW_UP = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_DOWN) == GLFW_PRESS) key_controls.KEY_ARROW_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_LEFT) == GLFW_PRESS) key_controls.KEY_ARROW_LEFT = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_RIGHT) == GLFW_PRESS) key_controls.KEY_ARROW_RIGHT = true;
    
        if (glfwGetKey(glfw_window_, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) key_controls.KEY_PAGE_DOWN = true;
        if (glfwGetKey(glfw_window_, GLFW_KEY_PAGE_UP) == GLFW_PRESS) key_controls.KEY_PAGE_UP = true;
    
        key_controls.timestamp_ = glfwGetTime();
    
        return key_controls;
    }
    
    std::string OpenGLContext::GetFontLocation() {
        return FileSystem::GetInstance().GetDirectoryAssets() + config_.font_file_path;
    }
    
    int OpenGLContext::ClearColor() {
        if (!is_inited_) return -1;
    
        glClearColor(0.0f, 0.4f, 0.8f, 0.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
        return 0;
    }
    
    int OpenGLContext::SwapBuffers() {
        if (!is_inited_) return -1;
    
        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();
        return 0;
    }

} 
} 
}
