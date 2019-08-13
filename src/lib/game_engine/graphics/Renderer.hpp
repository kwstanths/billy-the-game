#ifndef __Renderer_hpp__
#define __Renderer_hpp__

#include "game_engine/math/Types.hpp"
#include "game_engine/Controls.hpp"
#include "game_engine/ConsoleParser.hpp"
#include "game_engine/graphics/opengl/OpenGLContext.hpp"
#include "game_engine/graphics/opengl/OpenGLRenderer.hpp"
#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/opengl/OpenGLCamera.hpp"
#include "game_engine/utility/CircularBuffer.hpp"

#include "GraphicsTypes.hpp"
#include "GraphicsObject.hpp"
#include "Mesh.hpp"
#include "AssetManager.hpp"
#include "Light.hpp"

namespace game_engine {
    
    class GameEngine;
    class WorldSector;

namespace graphics {

#define GAME_ENGINE_RENDERER_MAX_OBJECTS 100

    class Renderer {
        friend game_engine::GameEngine;
        friend game_engine::WorldSector;
    public:
        Renderer();

        int Init(opengl::OpenGLContextConfig_t config);

        int Destroy();

        bool IsInited();

        void SetView();

        void StartFrame();

        void EndFrame();

        void SetWindowSize(size_t width, size_t height);

        KeyControls_t GetControlInput();

        /**
            Draws an object with full lightning. All Draw() calls will happen at the end of the frame. This means 
            that calling this Draw() on an object multiple times within a single frame, will result in drawing the 
            same object multiple times on the last object position/scaling/rotation
            @param rendering_object The object to draw
            @return 0 = OK, -1 = rendering_object is not properly initialised
        */
        int Draw(GraphicsObject * rendering_object);

        /**
            Draws an object with no lightning, only with a diffuse texture. The first diffuse texture specified will 
            be used as a diffuse texture
            @param rendering_object The object to draw
            @return 0 = OK
        */
        int DrawSimple(GraphicsObject * rendering_object);

        /**
            Draws a line on the z pane
            @param start Starting point
            @param stop Stopping point
            @param z_height The z pane height
            @param size The size of the line
            @param color The color of the line
            @return 0 = OK, -1 = Not initialised
        */
        int DrawLineXY(math::Point2D start, math::Point2D stop, float z_height, float size, glm::vec3 color);

        /**
            Draw a rectangle on the z pane
            @param rect The rectangle
            @param z_height The z pane height
            @param size The size of the line
            @param color The color of the line
            @return 0 = OK, -1 = Not initialised
        */
        int DrawRectangleXY(math::Rectangle2D rect, float z_height, float size, glm::vec3 color);

        /**
            Sets the directional light of the scene
            @param direction The light direction
            @param light_properties The properties of the light
            @return 0 = OK
        */
        int AddDirectionalLight(DirectionalLight * light);

        /**
            @param Sets a spotlight in the scene
            @param position The light position
            @param direction The light direction
            @param inner_angle The inner angle of the spotlight in degrees
            @param outer_angle The outer angle of the spotlight in degrees
            @param light_properties The light properties
            @param attenuation The attenuation of the light
            @return 0 = OK
        */
        int AddSpotLight(SpotLight * light);

        /**
        
        */
        int AddPointLight(PointLight * light);

        /**
            Draw 2D text
        */
        int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:

        struct TEXT_DRAW_t {
            std::string text_;
            Real_t x;
            Real_t y;
            Real_t scale;
            glm::vec3 color;
        };

        bool is_inited_;
        utility::CircularBuffer<PointLight *> point_lights_to_draw_;
        std::vector<GraphicsObject *> objects_to_draw_;
        utility::CircularBuffer<TEXT_DRAW_t> text_to_draw_;

        /* Variables needed for opengl drawiing */
        opengl::OpenGLContext * context_ = nullptr;
        opengl::OpenGLCamera * camera_ = nullptr;
        opengl::OpenGLRenderer * renderer_ = nullptr;

        enum RENDER_MODE {
            REGULAR,
            VIEW_FRUSTUM_CULLING,
            OCCLUSION_QUERIES
        };
        size_t frr_render_mode = RENDER_MODE::REGULAR;

        bool separable_ao = false;
        bool ssao_blur = true;
        bool draw_ssao_texture = false;

        /**
            Set a camera
            @param camera The camera to set
            @return 0=OK, -1 = Not initialised
        */
        int SetCamera(opengl::OpenGLCamera * camera);

        /**
            Flushes the point lights into drawing, flushes the object draw calls into drawing
        */
        void FlushDrawCalls();
    };

}
}

#endif